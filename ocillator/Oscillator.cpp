//
// Created by anini on 03-04-25.
//
#include <cmath>
#include "Oscillator.h"

Oscillator::Oscillator(double sampleRate)
: sampleRate(sampleRate),
phase(0.0),
frequency(440.0),
frequencyOffset(0.0),
phaseStep(0.0),
waveform(Waveform::SAW){
    updatePhaseStep();
}

void Oscillator::setFrequency(double frequency) {
    this->frequency = frequency;
    updatePhaseStep();
}
void Oscillator::setWaveform(Waveform waveform) {
this->waveform = waveform;
}
void Oscillator::updatePhaseStep() {
    double effectiveFrequency = frequency+frequencyOffset;
    phaseStep = 2.0 *M_PI * effectiveFrequency/sampleRate;
}

void Oscillator::setFrequencyOffset(double offset) {
    this->frequencyOffset = offset;
    updatePhaseStep();

}

void Oscillator::generate(float *buffer, int frames) {
    for (int i = 0; i < frames; ++i) {
        float sample = 0.0f;
        switch (waveform) {
            case SINE:
                sample = static_cast<float>(0.5 * sin(phase));
            break;

            case SQUARE:
                sample = sin(phase) >= 0.0 ? 0.5f : -0.5f;
            break;

            case SAW:
            sample = static_cast<float>((2.0 * (phase/(2.0 * M_PI))) -1.0) * 0.5f;
            break;

        }
        buffer[2 * i] += sample;
        buffer[2 * i + 1] += sample;

        phase += phaseStep;
        if (phase >=2.0* M_PI) {
            phase -= 2.0*M_PI;
        }
    }
}







