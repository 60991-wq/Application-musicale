#include "Oscillator.h"
#include <cmath>
#include <iostream>
#include "../util/Constants.h" // Pour SAMPLE_RATE et TWO_PI

Oscillator::Oscillator(double sampleRate)
    : sampleRate(sampleRate),
      frequency(440.0),
      frequencyOffset(0.0),
      phase(0.0),
      waveform(Waveform::SINE),
      filter(sampleRate)
{
    updatePhaseStep();
}

void Oscillator::setFrequency(double hz) {
    frequency = hz;
    updatePhaseStep();
}

void Oscillator::setWaveform(Waveform wf) {
    waveform = wf;
}

void Oscillator::setFrequencyOffset(double offset) {
    frequencyOffset = offset;
    updatePhaseStep();
}

void Oscillator::updatePhaseStep() {
    double effectiveFreq = frequency + frequencyOffset;
    phaseStep = TWO_PI * effectiveFreq / sampleRate;
}

void Oscillator::process(float* buffer, int frames) {
    for (int i = 0; i < frames; ++i) {
        float sample = 0.0f;

        // Générer la forme d'onde
        switch (waveform) {
            case Waveform::SINE:
                sample = 0.5f * std::sin(phase);
            break;
            case Waveform::SQUARE:
                sample = (std::sin(phase) >= 0.0) ? 0.5f : -0.5f;
            break;
            case Waveform::SAW:
                sample = static_cast<float>((1.0 - (phase / TWO_PI)) * 2.0 - 1.0) * 0.5f;
            break;
        }

        // Appliquer filtre simple
        sample = filter.process(sample);

        buffer[i] = sample;

        phase += phaseStep;
        if (phase >= TWO_PI)
            phase -= TWO_PI;
    }
}
