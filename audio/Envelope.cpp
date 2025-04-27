#include "Envelope.h"
#include <algorithm>

Envelope::Envelope(double sampleRate)
    : sampleRate(sampleRate),
      envelopeValue(0.0),
      attackTime(0.1),
      releaseTime(0.5),
      attackIncrement(0.0),
      releaseIncrement(0.0),
      state(State::IDLE)
{
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
    if (state == State::SUSTAIN || state == State::ATTACK) {
        state = RELEASE;
    }
}

double Envelope::getValue() const {
    return envelopeValue;
}

void Envelope::update() {
    switch (state) {
        case ATTACK:
            envelopeValue += attackIncrement;
        if (envelopeValue >= 1.0) {
            envelopeValue = 1.0;
            state = SUSTAIN;
        }
        break;
        case SUSTAIN:
            envelopeValue = 1.0;
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

void Envelope::process(float* buffer, int frames) {
    for (int i = 0; i < frames; ++i) {
        update();
        float gain = static_cast<float>(getValue());
        buffer[i] *= gain; // Appliquer gain MONO
    }
}

void Envelope::updateIncrements() {
    attackIncrement = (attackTime > 0.0) ? (1.0 / (attackTime * sampleRate)) : 1.0;
    releaseIncrement = (releaseTime > 0.0) ? (1.0 / (releaseTime * sampleRate)) : 1.0;
}
