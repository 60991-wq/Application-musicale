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

    explicit Envelope();

    void setSampleRate(double newSampleRate);

    void setParameters(double attackTimeSeconds, double releaseTimeSeconds);

    void noteOn();

    void noteOff();

    void process(float *audioBuffer);

    bool isRunning() const;

private:
    void enterState(State newState);

    State currentState;

    double sampleRate;
    double envelopeValue;

    double attackDurationSeconds;
    double releaseDurationSeconds;

    int elapsedSamplesInStage;
    int totalSamplesInStage;
};

#endif // ENVELOPE_H
