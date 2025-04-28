#include "Envelope.h"
#include <algorithm> // Pour std::clamp

Envelope::Envelope(double sampleRate)
    : sampleRate(sampleRate),
      currentState(State::IDLE),
      envelopeValue(0.0),
      attackTime(0.1),
      releaseTime(0.5),
      sampleCounter(0),
      samplesInCurrentStage(0)
{
}

void Envelope::setSampleRate(double rate) {
    sampleRate = rate;
}

void Envelope::setParameters(double attackTimeSeconds, double releaseTimeSeconds) {
    attackTime = attackTimeSeconds;
    releaseTime = releaseTimeSeconds;
}

void Envelope::noteOn() {
    enterState(State::ATTACK);
}

void Envelope::noteOff() {
    if (currentState == State::ATTACK || currentState == State::SUSTAIN) {
        enterState(State::RELEASE);
    }
}

bool Envelope::isRunning() const {
    return currentState != State::IDLE;
}

void Envelope::enterState(State newState) {
    currentState = newState;
    sampleCounter = 0;

    switch (currentState) {
        case State::ATTACK:
            samplesInCurrentStage = static_cast<int>(attackTime * sampleRate);
        break;
        case State::RELEASE:
            samplesInCurrentStage = static_cast<int>(releaseTime * sampleRate);
        break;
        case State::SUSTAIN:
        case State::IDLE:
            samplesInCurrentStage = 0;
        break;
    }
}

void Envelope::process(float* buffer, int frames) {
    for (int i = 0; i < frames; ++i) {
        switch (currentState) {
            case State::ATTACK:
                if (samplesInCurrentStage > 0) {
                    envelopeValue = static_cast<double>(sampleCounter) / samplesInCurrentStage;
                } else {
                    envelopeValue = 1.0;
                }
                sampleCounter++;
                if (sampleCounter >= samplesInCurrentStage) {
                    enterState(State::SUSTAIN);
                }
                break;

                case State::SUSTAIN:
                envelopeValue = 1.0;
                break;

            case State::RELEASE:
                if (samplesInCurrentStage > 0) {
                    envelopeValue = 1.0 - (static_cast<double>(sampleCounter) / samplesInCurrentStage);
                } else {
                    envelopeValue = 0.0;
                }
                sampleCounter++;
                if (sampleCounter >= samplesInCurrentStage) {
                    enterState(State::IDLE);
                }
                break;

                case State::IDLE:
                envelopeValue = 0.0;
                break;
        }

        // Clamp pour éviter les dépassement
        envelopeValue = std::clamp(envelopeValue, 0.0, 1.0);
        buffer[2 * i] *= static_cast<float>(envelopeValue);
        buffer[2 * i + 1] *= static_cast<float>(envelopeValue);    }
}
