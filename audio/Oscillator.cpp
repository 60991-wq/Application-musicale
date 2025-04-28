#include "Oscillator.h"
#include <cmath>
#include <iostream>
#include "../util/Constants.h" // Pour SAMPLE_RATE et TWO_PI

Oscillator::Oscillator(double sampleRate, float noteFreq)
    : sampleRate(sampleRate),
      frequency(noteFreq),
      phase(0.0),
      waveform(Waveform::SINE) {
}
void Oscillator::setFrequency(double hz) {
    frequency = hz;
}

void Oscillator::setWaveform(Waveform wf) {
    waveform = wf;
}
void Oscillator::resetPhase() {
    phase = 0.0;
}
void Oscillator::setSampleRate(double rate) {
    sampleRate = rate;
}


void Oscillator::process(float* buffer, int frames) {
    // Calculer le pas de phase
    double phaseStep = Constants::TwoPi * frequency / sampleRate;

    for (int i = 0; i < frames; ++i) {
        float sample = 0.0f;

        switch (waveform) {
            case Waveform::SINE:
                sample = std::sin(phase);
            break;
            case Waveform::SQUARE:
                sample = (phase < M_PI) ? 1.0f : -1.0f;
            break;
            case Waveform::SAW:
                sample = 2.0f * (phase / Constants::TwoPi) - 1.0f;
            break;
        }

        buffer[2 * i] = sample * 0.5f;
        buffer[2 * i + 1] = sample * 0.5f;

        phase += phaseStep;
        if (phase >= Constants::TwoPi)
            phase -= Constants::TwoPi;
    }
}