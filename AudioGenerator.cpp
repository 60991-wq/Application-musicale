#include "AudioGenerator.h"
#include "util/Constants.h"
#include <iostream>
#include <algorithm>
#include <cmath>

#include "audio/Delay.h"

// Constructeur : initialise les références et les modules audio
AudioGenerator::AudioGenerator(LockedPOD &sharedParams)
               : params(sharedParams){}

// Fonction d'initialisation PortAudio
void AudioGenerator::init() {
    PaError errorInit = Pa_Initialize();
    if (errorInit != paNoError) {
        std::cerr << "PortAudio error in Pa_Initialize(): " << Pa_GetErrorText(errorInit) << std::endl;
        return;
    }

    PaError errorStream = Pa_OpenDefaultStream(
        &stream,
        0, 2, paFloat32,
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

int AudioGenerator::audioCallback(const void*, void* outputBuffer,
                                  unsigned long framesPerBuffer,
                                  const PaStreamCallbackTimeInfo*,
                                  PaStreamCallbackFlags,
                                  void* userData) {
    auto* generator = static_cast<AudioGenerator*>(userData); // userData est toujours AudioGenerator*
    float* out = static_cast<float*>(outputBuffer);

    // Variables statiques pour les modules audio
    static Oscillator osc1;
    static Oscillator osc2;
    static Envelope envelope(Constants::SampleRate);
    static Filter filter;
    static Delay delay;
    static double currentTimeInSeconds = 0.0;

    POD paramsSnapshot = generator->params.getCopy();

    static bool previousNoteState = false;
    bool currentNoteState = paramsSnapshot.activeNote;

    if (currentNoteState && !previousNoteState) {
        osc1.resetPhase();
        osc2.resetPhase();
        envelope.noteOn();
    }
    else if (!currentNoteState && previousNoteState) {
        envelope.noteOff();
    }
    previousNoteState = currentNoteState;

    float baseFrequency = 261.63f;
    float noteFreq = baseFrequency * std::pow(2.0f, paramsSnapshot.noteIndex / 12.0f);

    osc1.setFrequency(noteFreq + paramsSnapshot.osc1OFrequencyOffset);
    osc1.setWaveform(static_cast<Oscillator::Waveform>(paramsSnapshot.osc1Waveform));
    osc2.setFrequency(noteFreq);

    // l'oscillateur 2 doit toujours être en mode dent de scie (SAW)
    osc2.setWaveform(Oscillator::Waveform::SAW);

    envelope.setParameters(paramsSnapshot.attack, paramsSnapshot.release);

    // --- Génération des signaux MONO ---
    float bufferOsc1[Constants::FramesPerBuffer];
    float bufferOsc2[Constants::FramesPerBuffer];
    float mixBuffer[Constants::FramesPerBuffer];

    std::fill(bufferOsc1, bufferOsc1 + framesPerBuffer, 0.0f);
    std::fill(bufferOsc2, bufferOsc2 + framesPerBuffer, 0.0f);
    std::fill(mixBuffer, mixBuffer + framesPerBuffer, 0.0f);

    // Générer les sons des oscillateurs seulement s'ils sont actifs
    if (paramsSnapshot.osc1Active)
        osc1.process(bufferOsc1, framesPerBuffer);

    if (paramsSnapshot.osc2Active)
        osc2.process(bufferOsc2, framesPerBuffer);

    // Mixage MONO des deux oscillateurs
    for (unsigned long i = 0; i < framesPerBuffer; ++i) {
        mixBuffer[i] = 0.5f * (bufferOsc1[i] + bufferOsc2[i]);
    }

    // Application de l'enveloppe MONO
    envelope.process(mixBuffer, framesPerBuffer);

    // Application du filtre MONO
    filter.setCutoff(paramsSnapshot.cutoff);
    filter.setResonance(paramsSnapshot.resonance);
    filter.process(mixBuffer, framesPerBuffer);

    // Application du delay MONO
    delay.setDelayTime(paramsSnapshot.delayTime);
    delay.setMix(paramsSnapshot.delayMix);
    delay.process(mixBuffer, framesPerBuffer);

    // Conversion MONO vers STÉRÉO entrelacé pour la sortie
    for (unsigned long i = 0; i < framesPerBuffer; ++i) {
        out[2*i] = mixBuffer[i];      // Canal gauche
        out[2*i+1] = mixBuffer[i];    // Canal droit
    }

    // Mise à jour du temps
    currentTimeInSeconds += framesPerBuffer / static_cast<double>(Constants::SampleRate);

    return paContinue;
}