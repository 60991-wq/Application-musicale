//
// Created by aninia on 25-04-25.
//
#ifndef ENVELOPE_H
#define ENVELOPE_H

class Envelope {
public:
    enum class State {
        IDLE,
        ATTACK,
        SUSTAIN,
        RELEASE,
    };

    void setParameters(float attackTimeSeconds, float releaseTimeSeconds);

    void noteOn();

    void noteOff();


    void process(float *audioBuffer);

private:
    float attackDuration{0.5f};
    float releaseDuration{1.0f};
    float currentLevel{0.0f};
    State currentPhase{State::IDLE};

    int frameCounter = 0;
    int attackFrames = 0;
    int releaseFrames = 0;
};

#endif // ENVELOPE_H
