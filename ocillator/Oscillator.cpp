//
// Created by anini on 03-04-25.
//
#include <cmath>
#include "Oscillator.h"
#include <iostream>

namespace {
    // Fonction utilitaire pour convertir une waveform en texte lisible
    const char *waveformToString(Oscillator::Waveform wf) {
        switch (wf) {
            case Oscillator::Waveform::SINE: return "SINE";
            case Oscillator::Waveform::SQUARE: return "SQUARE";
            case Oscillator::Waveform::SAW: return "SAW";
            default: return "UNKNOWN";
        }
    }
}


Oscillator::Oscillator(double sampleRate)
    : sampleRate(sampleRate),
      phase(0.0),
      frequency(440.0),
      frequencyOffset(0.0),
      phaseStep(0.0),
      envelope(sampleRate),
      waveform(Waveform::SQUARE){
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
    double effectiveFrequency = frequency + frequencyOffset;
    phaseStep = 2.0 * M_PI * effectiveFrequency / sampleRate;
}

void Oscillator::setFrequencyOffset(double offset) {
    this->frequencyOffset = offset;
    updatePhaseStep();
}

void Oscillator::generate(float *buffer, int frames) {
    for (int i = 0; i < frames; ++i) {
        float sample = 0.0f;
        switch (waveform) {
            case Waveform::SINE:
                sample = static_cast<float>(0.5 * sin(phase));
                break;

            case Waveform::SQUARE:
                sample = sin(phase) >= 0.0 ? 0.5f : -0.5f;
                break;

            case Waveform::SAW:
                sample = static_cast<float>((1.0 - (phase / (M_PI))) * 0.5);
                break;
        }

        float amp = static_cast<float>(envelope.getValue());

        buffer[2 * i] += sample;
        buffer[2 * i + 1] += sample;

        phase += phaseStep;
        if (phase >= 2.0 * M_PI) {
            phase -= 2.0 * M_PI;
        }
    }
    // me permet de savoir quel onde je joue j'attribut un num a chaque onde pour tester si ca change bien
    static bool shown = false;
    if (!shown) {
        std::cout << "Waveform utilisée : " << waveformToString << std::endl;
        shown = true;
    }


}

void Oscillator::noteOff() {
    this->waveform = Waveform::SINE;
}
void Oscillator::noteOn() {
    this->waveform = Waveform::SINE;

}

