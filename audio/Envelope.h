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

    explicit Envelope(double sampleRate = 44100.0);

    void setSampleRate(double rate);
    void setParameters(double attackTimeSeconds, double releaseTimeSeconds);

    void noteOn();
    void noteOff();

    void process(float* buffer);
    bool isRunning() const;

private:
    void enterState(State newState);

    State currentState;

    double sampleRate;
    double envelopeValue;

    double attackTime;     // secondes
    double releaseTime;    // secondes

    int sampleCounter;
    int samplesInCurrentStage;
};

#endif // ENVELOPE_H
