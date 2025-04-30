#include "Oscillator.h"
#include <cmath>
#include <iostream>
#include "../util/Constants.h" // Pour SAMPLE_RATE et TWO_PI

Oscillator::Oscillator()
    : sampleRate(Constants::SampleRate),
      frequency(440.0f),
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

    double phaseStep = Constants::TwoPi * frequency / sampleRate;
    for (int i = 0; i < frames; ++i) {
        float sample = 0.0f;

        switch (waveform) {
            case Waveform::SINE:
                sample = static_cast<float>(sin(phase));
            break;
            case Waveform::SQUARE:
                sample = (phase < M_PI) ? 1.0f : -1.0f;
            break;
            case Waveform::SAW:
                sample = static_cast<float>(2.0 * (phase / Constants::TwoPi) - 1.0);
            break;
        }

        buffer[i] = sample;
        phase += phaseStep;
        if (phase >= Constants::TwoPi)
            phase -= Constants::TwoPi;
    }
}