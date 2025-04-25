#include "AudioGenerator.h"
#include "util/Constants.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>



AudioGenerator::AudioGenerator(LockedPOD& sharedParams)
    : callbackData{&sharedParams}
{}

void AudioGenerator::init() {
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio initialization failed: " << Pa_GetErrorText(err) << std::endl;
        return;
    }

    err = Pa_OpenDefaultStream(&stream,
                                0, 2, paFloat32,
                                SAMPLE_RATE,
                                FRAMES_PER_BUFFER,
                                audioCallback,
                                &callbackData);
    if (err != paNoError) {
        std::cerr << "Failed to open PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
        return;
    }

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cerr << "Failed to start PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
    }
}

int AudioGenerator::audioCallback(const void*,
                                  void* outputBuffer,
                                  unsigned long framesPerBuffer,
                                  const PaStreamCallbackTimeInfo*,
                                  PaStreamCallbackFlags,
                                  void* userData) {

    auto* callbackData = static_cast<AudioCallbackData*>(userData);
    if (!callbackData || !callbackData->lockedParams) {
        return paContinue;
    }

    POD params = callbackData->lockedParams->getCopy();
    float* out = reinterpret_cast<float*>(outputBuffer);

    static Oscillator osc1(SAMPLE_RATE);
    static Oscillator osc2(SAMPLE_RATE);
    static Envelope envelope(SAMPLE_RATE);
    static bool noteWasPressed = false;

    bool noteNowPressed = (params.activeNote != -1);
    if (noteNowPressed && !noteWasPressed) {
        envelope.noteOn();
    } else if (!noteNowPressed && noteWasPressed) {
        envelope.noteOff();
    }
    noteWasPressed = noteNowPressed;

    // Fréquence de base C4 (Do)
    constexpr float baseFreq = 261.63f;
    float noteFreq = baseFreq * std::pow(2.0f, params.activeNote / 12.0f);


    osc1.setWaveform(static_cast<Oscillator::Waveform>(params.osc1Waveform));
    osc1.setFrequency(noteFreq);
    osc1.setSampleRate(SAMPLE_RATE);

    osc2.setWaveform(static_cast<Oscillator::Waveform>(params.osc1Waveform)); // (Tu peux différencier si tu veux)
    osc2.setFrequency(noteFreq);
    osc2.setSampleRate(SAMPLE_RATE);

    envelope.setAttackTime(params.attack);
    envelope.setReleaseTime(params.release);

    std::vector<float> buffer1(framesPerBuffer, 0.0f);
    std::vector<float> buffer2(framesPerBuffer, 0.0f);
    std::vector<float> mixedBuffer(framesPerBuffer, 0.0f);

    if (params.osc1Active) {
        osc1.generate(buffer1.data(), static_cast<int>(framesPerBuffer));
    }

    if (params.osc2Active) {
        osc2.generate(buffer2.data(), static_cast<int>(framesPerBuffer));
    }

    for (unsigned long i = 0; i < framesPerBuffer; ++i) {
        mixedBuffer[i] = 0.5f * (buffer1[i] + buffer2[i]);
    }

    for (unsigned long i = 0; i < framesPerBuffer; ++i) {
        envelope.update();
        mixedBuffer[i] *= static_cast<float>(envelope.getValue());
    }

    for (unsigned long i = 0; i < framesPerBuffer; ++i) {
        out[2 * i]     = mixedBuffer[i];
        out[2 * i + 1] = mixedBuffer[i];
    }

    return paContinue;
}