#include "AudioGenerator.h"
#include "util/Constants.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

constexpr int CALLBACK_LOG_INTERVAL = 100;

AudioGenerator::AudioGenerator(LockedPOD& sharedParams)
    : callbackData{&sharedParams},
      osc1(SAMPLE_RATE),
      osc2(SAMPLE_RATE),
      envelope(SAMPLE_RATE),
      filter(SAMPLE_RATE)
{}

void AudioGenerator::init() {
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "Échec de l'initialisation PortAudio : " << Pa_GetErrorText(err) << std::endl;
        return;
    }
    std::cout << "PortAudio initialisé avec succès" << std::endl;

    err = Pa_OpenDefaultStream(&stream,
                               0, 2, paFloat32,
                               SAMPLE_RATE,
                               FRAMES_PER_BUFFER,
                               audioCallback,
                               this);
    if (err != paNoError) {
        std::cerr << "Erreur ouverture flux PortAudio : " << Pa_GetErrorText(err) << std::endl;
        return;
    }
    std::cout << "Flux PortAudio ouvert avec succès" << std::endl;

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cerr << "Erreur démarrage flux PortAudio : " << Pa_GetErrorText(err) << std::endl;
        return;
    }
    std::cout << "Flux PortAudio démarré avec succès" << std::endl;
}

void AudioGenerator::cleanup() {
    std::cout << "Nettoyage des ressources PortAudio" << std::endl;
    if (stream) {
        Pa_StopStream(stream);
        Pa_CloseStream(stream);
        Pa_Terminate();
    }
    std::cout << "Nettoyage terminé" << std::endl;
}int AudioGenerator::audioCallback(const void*,
                                  void* outputBuffer,
                                  unsigned long framesPerBuffer,
                                  const PaStreamCallbackTimeInfo*,
                                  PaStreamCallbackFlags,
                                  void* userData) {
    static int callCount = 0;
    if (callCount++ % CALLBACK_LOG_INTERVAL == 0) {
        std::cout << "Appel audioCallback (" << callCount << ")" << std::endl;
    }

    auto* generator = static_cast<AudioGenerator*>(userData);
    auto* callbackData = &generator->callbackData;

    if (!callbackData || !callbackData->lockedParams) {
        std::cerr << "Callback invalide" << std::endl;
        return paAbort;
    }

    POD params = callbackData->lockedParams->getCopy();
    float* out = reinterpret_cast<float*>(outputBuffer);

    bool noteNowPressed = (params.activeNote != -1);

    if (noteNowPressed && !generator->noteWasPressed) {
        std::cout << "Note ON: " << params.activeNote << std::endl;
        generator->envelope.noteOn();
    } else if (!noteNowPressed && generator->noteWasPressed) {
        std::cout << "Note OFF" << std::endl;
        generator->envelope.noteOff();
    }
    generator->noteWasPressed = noteNowPressed;

    if (!noteNowPressed) {
        static float phase = 0.0f;
        for (unsigned long i = 0; i < framesPerBuffer; ++i) {
            float sample = 0.3f * sinf(phase);
            phase += 0.1f;
            if (phase >= 2.0f * M_PI) phase -= 2.0f * M_PI;

            out[2 * i] = sample;
            out[2 * i + 1] = sample;
        }
        return paContinue;
    }

    constexpr float A4_FREQ = 440.0f;
    constexpr int A4_MIDI_NOTE = 69;
    float noteFreq = A4_FREQ * std::pow(2.0f, (params.activeNote - A4_MIDI_NOTE) / 12.0f);

    generator->osc1.setWaveform(static_cast<Oscillator::Waveform>(params.osc1Waveform));
    generator->osc1.setFrequency(noteFreq + params.osc1Offset);
    generator->osc2.setWaveform(Oscillator::Waveform::SAW);
    generator->osc2.setFrequency(noteFreq);

    generator->envelope.setAttackTime(params.attack);
    generator->envelope.setReleaseTime(params.release);

    std::vector<float> buffer1(framesPerBuffer, 0.0f); // mono
    std::vector<float> buffer2(framesPerBuffer, 0.0f); // mono
    std::vector<float> mixedBuffer(framesPerBuffer, 0.0f); // mono

    if (params.osc1Active) {
        generator->osc1.process(buffer1.data(), static_cast<int>(framesPerBuffer));
    }
    if (params.osc2Active) {
        generator->osc2.process(buffer2.data(), static_cast<int>(framesPerBuffer));
    }

    for (unsigned long i = 0; i < framesPerBuffer; ++i) {
        mixedBuffer[i] = 0.5f * (buffer1[i] + buffer2[i]);
    }

    generator->envelope.process(mixedBuffer.data(), static_cast<int>(framesPerBuffer));

    // Copier en stéréo
    for (unsigned long i = 0; i < framesPerBuffer; ++i) {
        out[2 * i] = mixedBuffer[i];      // Gauche
        out[2 * i + 1] = mixedBuffer[i];  // Droite
    }

    return paContinue;
}
