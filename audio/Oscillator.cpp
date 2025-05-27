#include "Oscillator.h"
#include <cmath>
#include <iostream>
#include "../util/Constants.h" // Contient SampleRate et TwoPi

Oscillator::Oscillator()
    : currentSampleRate(Constants::SampleRate),
      currentFrequencyHz(440.0),
      phaseRadians(0.0),
      waveformType(Waveform::SINE) {
}

void Oscillator::setFrequency(double newFrequencyHz) {
    currentFrequencyHz = newFrequencyHz;
}

void Oscillator::setWaveform(Waveform newWaveform) {
    waveformType = newWaveform;
}

void Oscillator::resetPhase() {
    phaseRadians = 0.0;
}

void Oscillator::setSampleRate(double newSampleRate) {
    currentSampleRate = newSampleRate;
}

void Oscillator::process(float* audioBuffer) {
    double phaseIncrement = Constants::TwoPi * currentFrequencyHz / currentSampleRate;

    for (int i = 0; i < Constants::FramesPerBuffer; ++i) {
        float sampleValue = 0.0f;

        switch (waveformType) {
            case Waveform::SINE:
                sampleValue = static_cast<float>(sin(phaseRadians));
            break;

            case Waveform::SQUARE:
                sampleValue = (phaseRadians < M_PI) ? -1.0f : 1.0f;
            break;

            case Waveform::SAW:
                sampleValue = static_cast<float>((2.0 * (phaseRadians / Constants::TwoPi)) - 1.0);
            break;
        }

        audioBuffer[i] = sampleValue;

        phaseRadians += phaseIncrement;
        if (phaseRadians >= Constants::TwoPi)
            phaseRadians -= Constants::TwoPi;
    }
}
