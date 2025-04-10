//
// Created by anini on 03-04-25.
//

#include "Envelope.h"
/*
Envelope::Envelope(double sampleRate)
    : sampleRate(sampleRate),
      envelopeValue(0.0),
      attackTime(0.1),
      releaseTime(0.5),
      attackIncrement(0.0),
      releaseIncrement(0.0),
      sustainLevel(0.7),
      state(IDLE) {
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

void Envelope::setSustainLevel(double level) {
    sustainLevel = level;
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
                state = RELEASE;
            }
            break;

        case DECAY:
            envelopeValue -= decayIncrement;
        if (envelopeValue <= sustainLevel) {
            envelopeValue = sustainLevel;
            state = SUSTAIN;
        }
        break;


        case RELEASE:
            envelopeValue -= releaseIncrement;
            if (envelopeValue <= 0.0) {
                envelopeValue = 0.0;
                state = IDLE;
            }
            break;

        case IDLE:
            break;
    }
}


void Envelope::updateIncrements() {
    attackIncrement = (attackTime > 0.0) ?(1.0/(attackTime * sampleRate)):1.0;
    releaseIncrement = (releaseTime > 0.0) ?(1.0/(releaseTime * sampleRate)):1.0;
}
*/