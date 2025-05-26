#include "AudioGenerator.h"
#include "util/Constants.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include "audio/Envelope.h"

#include "audio/Delay.h"
AudioGenerator::AudioGenerator(LockedSynthParameters &sharedParams)
    : params(sharedParams), wavOut("dump.wav",Constants::SampleRate)
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
    auto* generator = static_cast<AudioGenerator*>(userData);
    float* out = static_cast<float*>(outputBuffer);

    SynthParameters paramsSnapshot = generator->params.getCopy();

    bool currentNoteState = paramsSnapshot.activeNote;

    if (currentNoteState && !generator->previousNoteState) {
        generator->osc1.resetPhase();
        generator->osc2.resetPhase();
        generator->envelope.noteOn();
    } else if (!currentNoteState && generator->previousNoteState) {
        generator->envelope.noteOff();
    }
    generator->previousNoteState = currentNoteState;

    float baseFrequency = 261.63f;
    float noteFreq = baseFrequency * std::pow(2.0f, paramsSnapshot.noteIndex / 12.0f);

    generator->osc1.setFrequency(noteFreq + paramsSnapshot.osc1OFrequencyOffset);
    generator->osc1.setWaveform(static_cast<Oscillator::Waveform>(paramsSnapshot.osc1Waveform));

    generator->osc2.setFrequency(noteFreq);
    generator->osc2.setWaveform(Oscillator::Waveform::SAW);

    generator->envelope.setParameters(paramsSnapshot.attack, paramsSnapshot.release);

    float bufferOsc1[Constants::FramesPerBuffer]{};
    float bufferOsc2[Constants::FramesPerBuffer]{};
    float mixBuffer[Constants::FramesPerBuffer]{};

    if (paramsSnapshot.osc1Active)
        generator->osc1.process(bufferOsc1);
    if (paramsSnapshot.osc2Active)
        generator->osc2.process(bufferOsc2);

    for (unsigned long i = 0; i < framesPerBuffer; ++i)
        mixBuffer[i] = 0.5f * (bufferOsc1[i] + bufferOsc2[i]);

    generator->envelope.process(mixBuffer);

    generator->filter.setCutoff(paramsSnapshot.cutoff);
    generator->filter.setResonance(paramsSnapshot.resonance);
    generator->filter.process(mixBuffer);

    generator->delay.setDelayTime(paramsSnapshot.delayTime);
    generator->delay.setMix(paramsSnapshot.delayMix);
    generator->delay.process(mixBuffer);

    for (unsigned long i = 0; i < framesPerBuffer; ++i) {
        out[2*i]     = mixBuffer[i];
        out[2*i + 1] = mixBuffer[i];
        generator->wavOut.push_frame(out[2*i], out[2*i + 1]);
    }

    generator->currentTimeInSeconds += framesPerBuffer / static_cast<double>(Constants::SampleRate);

    return paContinue;
}
