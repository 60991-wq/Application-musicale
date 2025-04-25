//
// Created by anini on 03-04-25.
//

#include "Envelope.h"
#include <algorithm>

Envelope::Envelope(double sampleRate)
:  sampleRate(sampleRate),
  envelopeValue(0.0),
  attackTime(0.1),
  releaseTime(0.5),
  attackIncrement(0.0),
  releaseIncrement(0.0),
  state(State::IDLE) {
    updateIncrements();
}

void Envelope::setAttackTime(double seconds) {
    attackTime = seconds;
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
        case State::ATTACK:
            envelopeValue += attackIncrement;
            if (envelopeValue >= 1.0) {
                envelopeValue = 1.0;
                state = State::SUSTAIN;
            }
            break;
        case State::SUSTAIN:
            envelopeValue = 1.0;
        break;


        case  State::RELEASE:
            envelopeValue -= releaseIncrement;
            if (envelopeValue <= 0.0) {
                envelopeValue = 0.0;
                state = State::IDLE;
            }
            break;

        case State::IDLE:
            envelopeValue = 0.0;
            break;
    }
    envelopeValue = std::clamp(envelopeValue, 0.0, 1.0);
}


void Envelope::updateIncrements() {
    attackIncrement = (attackTime > 0.0) ? (1.0 / (attackTime * sampleRate)) : 1.0;
    releaseIncrement= (releaseTime > 0.0)? (1.0 / (releaseTime * sampleRate)) : 1.0;
}
