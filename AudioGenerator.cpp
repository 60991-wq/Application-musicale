#include "AudioGenerator.h"
#include "util/Constants.h"
#include <iostream>
#include <algorithm>
#include <cmath>

// Constructeur : initialise les références et les modules audio
AudioGenerator::AudioGenerator(LockedPOD &sharedParams)
    : params(sharedParams),
      osc1(Constants::SampleRate, 440.0f),
      osc2(Constants::SampleRate, 440.0f),
      envelope(Constants::SampleRate)
{
}

// Fonction d'initialisation PortAudio
void AudioGenerator::init() {
    PaError errorInit = Pa_Initialize();
    if (errorInit != paNoError) {
        std::cerr << "PortAudio error in Pa_Initialize(): " << Pa_GetErrorText(errorInit) << std::endl;
        return;
    }

    PaError errorStream = Pa_OpenDefaultStream(
        &stream,
        0, 2, paFloat32,               // 0 input, 2 outputs (stereo), format 32-bit float
        Constants::SampleRate,
        Constants::FramesPerBuffer,
        &AudioGenerator::audioCallback, // Callback audio
        this                            // Passage de l'objet AudioGenerator
    );

    if (errorStream != paNoError) {
        std::cerr << "PortAudio error in Pa_OpenDefaultStream(): " << Pa_GetErrorText(errorStream) << std::endl;
        return;
    }

    errorStream = Pa_StartStream(stream);
    if (errorStream != paNoError) {
        std::cerr << "PortAudio error in Pa_StartStream(): " << Pa_GetErrorText(errorStream) << '\n';
    }
}

// Fonction appelée automatiquement par PortAudio pour générer le son
int AudioGenerator::audioCallback(const void*, void* outputBuffer,
                                  unsigned long framesPerBuffer,
                                  const PaStreamCallbackTimeInfo*,
                                  PaStreamCallbackFlags,
                                  void* userData) {
    auto* generator = static_cast<AudioGenerator*>(userData);
    float* out = static_cast<float*>(outputBuffer);

    // --- Copie sécurisée de tous les paramètres actuels ---
    POD paramsSnapshot = generator->params.getCopy();

    // --- Détection de transition noteOn/noteOff ---
    static bool previousNoteState = false;
    bool currentNoteState = paramsSnapshot.activeNote;

    if (currentNoteState && !previousNoteState) {
        generator->osc1.resetPhase();
        generator->osc2.resetPhase();
        generator->envelope.noteOn();
    } else if (!currentNoteState && previousNoteState) {
        generator->envelope.noteOff();
    }
    previousNoteState = currentNoteState;
    // --- Fin détection notes ---

    // --- Configuration des oscillateurs ---
    float baseFrequency = 261.63f; // C4 (Do central)
    float noteFreq = baseFrequency * std::pow(2.0f, paramsSnapshot.noteIndex / 12.0f); // CORRECT : division par 12

    generator->osc1.setFrequency(noteFreq + paramsSnapshot.osc1OFrequencyOffset);
    generator->osc2.setFrequency(noteFreq);

    generator->osc1.setWaveform(static_cast<Oscillator::Waveform>(paramsSnapshot.osc1Waveform));
    generator->osc2.setWaveform(static_cast<Oscillator::Waveform>(paramsSnapshot.osc2Waveform));

    // Pas besoin de refaire setSampleRate tout le temps (fait au constructeur)
    // Idem pour envelope

    generator->envelope.setParameters(paramsSnapshot.attack, paramsSnapshot.release);

    // --- Génération des signaux ---
    float bufferOsc1[2*Constants::FramesPerBuffer];
    float bufferOsc2[2*Constants::FramesPerBuffer];
    float mixBuffer[2*Constants::FramesPerBuffer];

    if (paramsSnapshot.osc1Active)
        generator->osc1.process(bufferOsc1, framesPerBuffer);
    else
        std::fill(bufferOsc1, bufferOsc1 + 2*framesPerBuffer, 0.0f);

    if (paramsSnapshot.osc2Active)
        generator->osc2.process(bufferOsc2, framesPerBuffer);
    else
        std::fill(bufferOsc2, bufferOsc2 + 2*framesPerBuffer, 0.0f);

    // Mixage 50% osc1 + 50% osc2
    for (unsigned long i = 0; i < framesPerBuffer; ++i) {
        mixBuffer[2*i] = 0.5f * (bufferOsc1[2*i] + bufferOsc2[2*i]);       // Canal gauche
        mixBuffer[2*i+1] = 0.5f * (bufferOsc1[2*i+1] + bufferOsc2[2*i+1]); // Canal droit
    }

    // Application de l'enveloppe
    generator->envelope.process(mixBuffer, framesPerBuffer);

    // Copie dans buffer de sortie (stéréo)
    for (unsigned long i = 0; i < 2*framesPerBuffer; ++i) {
        out[i] = mixBuffer[i];
    }

    return paContinue;
}