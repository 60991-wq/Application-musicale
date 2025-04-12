#include "Oscillator.h"
#include <cmath>
#include "../util/Constants.h" // pour SAMPLE_RATE et TWO_PI
#include "Envelope.h"
#include<iostream>

 Oscillator::Oscillator()
    : sampleRate(SAMPLE_RATE),
      frequency(440.0),
      frequencyOffset(0.0),
      phase(0.0),
      waveform(Waveform::SQUARE),
     envelope (SAMPLE_RATE)

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

void Oscillator::noteOn() {
     envelope.noteOn();
 }
void Oscillator::noteOff() {
     envelope.noteOff();
 }

void Oscillator::setEnvelopeParams(double attack, double decay, double sustain, double release) {
     envelope.setAttackTime(attack);
     envelope.setDecayTime(decay);
     envelope.setSustainLevel(sustain);
     envelope.setReleaseTime(release);
 }



void Oscillator::generate(float* buffer, int frames) {
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
                sample = static_cast<float>((1.0 - (phase / M_PI)) * 0.5);
            break;
        }

        float gain = static_cast<float>(envelope.getValue());
        envelope.update();
        sample *= gain;

        buffer[2 * i]     += sample;
        buffer[2 * i + 1] += sample;

        phase += phaseStep;
        if (phase >= TWO_PI)
            phase -= TWO_PI;
    }
     static bool shown = false;
     if (!shown) {
         std::cout << "[Oscillator] Waveform active : " << static_cast<int>(waveform)
                   << ", Frequency : " << frequency << " Hz" << std::endl;
         shown = true;
     }
}
