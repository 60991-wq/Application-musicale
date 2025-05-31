// Created by aninia on 25-04-25.

#include "Envelope.h"
#include "../util/Constants.h"

void Envelope::setParameters(float attackTimeSeconds, float releaseTimeSeconds) {
    attackDuration = attackTimeSeconds;
    releaseDuration = releaseTimeSeconds;

    attackFrames = static_cast<int>(Constants::SampleRate * attackDuration);
    releaseFrames = static_cast<int>(Constants::SampleRate * releaseDuration);
}

void Envelope::noteOn() {
    if (currentPhase == State::ATTACK) {
        return;
    }

    currentPhase = State::ATTACK;
    frameCounter = 0;

    if (currentLevel > 0.0f && attackFrames > 0) {
        frameCounter = static_cast<int>(static_cast<float>(attackFrames) * currentLevel);
    }
}

void Envelope::noteOff() {
    if (currentPhase == State::RELEASE || currentPhase == State::IDLE) {
        return;
    }

    currentPhase = State::RELEASE;
    frameCounter = 0;

    if (currentLevel < 1.0f && releaseFrames > 0) {
        frameCounter = static_cast<int>((1.0f - currentLevel) * static_cast<float>(releaseFrames));
    }
}

void Envelope::process(float *audioBuffer) {
    for (int frame = 0; frame < Constants::FramesPerBuffer; ++frame) {
        switch (currentPhase) {
            case State::ATTACK:
                if (attackFrames > 0) {
                    currentLevel = static_cast<float>(frameCounter) / static_cast<float>(attackFrames);
                    if (currentLevel >= 1.0f) {
                        currentLevel = 1.0f;
                        currentPhase = State::SUSTAIN;
                    } else {
                        frameCounter++;
                    }
                } else {
                    currentLevel = 1.0f;
                    currentPhase = State::SUSTAIN;
                }
                break;

            case State::SUSTAIN:
                currentLevel = 1.0f;
                break;

            case State::RELEASE:
                if (releaseFrames > 0) {
                    currentLevel = 1.0f - (static_cast<float>(frameCounter) /static_cast<float>(releaseFrames));
                    if (currentLevel <= 0.0f) {
                        currentLevel = 0.0f;
                        currentPhase = State::IDLE;
                    } else {
                        frameCounter++;
                    }
                } else {
                    currentLevel = 0.0f;
                    currentPhase = State::IDLE;
                }
                break;

            case State::IDLE:
                currentLevel = 0.0f;
                break;
        }

        audioBuffer[frame] *= currentLevel;
    }
}
