#include "AudioGenerator.h"
#include "util/Constants.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

AudioGenerator::AudioGenerator(LockedPOD& sharedParams)
    : callbackData{&sharedParams},
      osc1(SAMPLE_RATE),
      osc2(SAMPLE_RATE),
      filter(SAMPLE_RATE)
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
                                this);
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
    auto* generator = static_cast<AudioGenerator*>(userData);
    auto* callbackData = &generator->callbackData;

    if (!callbackData || !callbackData->lockedParams) {
        return paContinue;
    }

    POD params = callbackData->lockedParams->getCopy();
    float* out = reinterpret_cast<float*>(outputBuffer);

    // Gérer les notes (noteOn/noteOff)
    bool noteNowPressed = (params.activeNote != -1);
    if (noteNowPressed && !generator->noteWasPressed) {
        generator->osc1.noteOn();
        generator->osc2.noteOn();
    } else if (!noteNowPressed && generator->noteWasPressed) {
        generator->osc1.noteOff();
        generator->osc2.noteOff();
    }
    generator->noteWasPressed = noteNowPressed;

    // Fréquence de base C4 (Do)
    constexpr float baseFreq = 261.63f;
    float noteFreq = baseFreq;
    if (params.activeNote >= 0) {
        noteFreq = baseFreq * std::pow(2.0f, params.activeNote / 12.0f);
    }

    // Configurer oscillateurs
    generator->osc1.setWaveform(static_cast<Oscillator::Waveform>(params.osc1Waveform));
    generator->osc1.setFrequency(noteFreq + params.osc1Offset);
    generator->osc1.setEnvelopeParams(params.attack, params.release);

    generator->osc2.setWaveform(Oscillator::Waveform::SAW); // OSC2 toujours SAW
    generator->osc2.setFrequency(noteFreq);
    generator->osc2.setEnvelopeParams(params.attack, params.release);

    // Préparer buffers
    std::vector<float> buffer1(framesPerBuffer * 2, 0.0f);
    std::vector<float> buffer2(framesPerBuffer * 2, 0.0f);

    if (params.osc1Active) {
        generator->osc1.generate(buffer1.data(), static_cast<int>(framesPerBuffer));
    }
    if (params.osc2Active) {
        generator->osc2.generate(buffer2.data(), static_cast<int>(framesPerBuffer));
    }

    // Mix osc1 + osc2
    for (unsigned long i = 0; i < framesPerBuffer * 2; ++i) {
        out[i] = 0.5f * (buffer1[i] + buffer2[i]);
    }

    // Appliquer le filtre
    generator->filter.setCutoff(params.cutoff);
    for (unsigned long i = 0; i < framesPerBuffer * 2; ++i) {
        out[i] = generator->filter.process(out[i]);
    }

    return paContinue;
}
