//
// Created by anini on 03-04-25.
//

#include "Envelope.h"
#include <algorithm>

Envelope::Envelope(double sampleRate)
:  sampleRate(sampleRate),
  envelopeValue(0.0),
  attackTime(0.1),
  decayTime(0.1),
  sustainLevel(0.7),
  releaseTime(0.5),
  attackIncrement(0.0),
  decayIncrement(0.0),
  releaseIncrement(0.0),
  state(IDLE) {
    updateIncrements();
}

void Envelope::setAttackTime(double seconds) {
    attackTime = seconds;
    updateIncrements();
} void Envelope::setDecayTime(double seconds) {
    decayTime = seconds;
    updateIncrements();
}

void Envelope::setSustainLevel(double level) {
    sustainLevel = std::clamp(level, 0.0, 1.0);
    updateIncrements();
}

void Envelope::setReleaseTime(double seconds) {
    releaseTime = seconds;
    updateIncrements();
}

void Envelope::noteOn() {
    state = ATTACK;
}

void Envelope::noteOff() {
    state = RELEASE;
}

double Envelope::getValue() const {
    return envelopeValue;
}
void Envelope::update() {
    switch (state) {
        case ATTACK:
            envelopeValue += attackIncrement;
            if (envelopeValue >= 1.0) {
                envelopeValue -= 1.0;
                state = DECAY;
            }
            break;

        case DECAY:
            envelopeValue -= decayIncrement;
        if (envelopeValue <= sustainLevel) {
            envelopeValue = sustainLevel;
            state = SUSTAIN;
        }
        break;

        case SUSTAIN:
                envelopeValue = sustainLevel;
        break;


        case RELEASE:
            envelopeValue -= releaseIncrement;
            if (envelopeValue <= 0.0) {
                envelopeValue = 0.0;
                state = IDLE;
            }
            break;

        case IDLE:
            envelopeValue = 0.0;
            break;
    }
    envelopeValue = std::clamp(envelopeValue, 0.0, 1.0);
}


void Envelope::updateIncrements() {
    attackIncrement = (attackTime > 0.0) ? (1.0 / (attackTime * sampleRate)) : 1.0;
    decayIncrement  = (decayTime > 0.0)  ? ((1.0 - sustainLevel) / (decayTime * sampleRate)) : 1.0;
    releaseIncrement= (releaseTime > 0.0)? (sustainLevel / (releaseTime * sampleRate)) : 1.0;
}
