#ifndef ENVELOPE_H
#define ENVELOPE_H

class Envelope {
public:
    // États de l'enveloppe
    enum class State {
        IDLE,
        ATTACK,
        SUSTAIN,
        RELEASE,
    };

    explicit Envelope();

    void setSampleRate(double newSampleRate);
    void setParameters(double attackTimeSeconds, double releaseTimeSeconds);

    void noteOn();   // Déclenchement de la note (attaque)
    void noteOff();  // Fin de la note (relâchement)

    void process(float* audioBuffer);  // Applique l'enveloppe au buffer audio
    bool isRunning() const;            // Indique si une note est active

private:
    void enterState(State newState);   // Change d'état et initialise les compteurs

    State currentState;

    double sampleRate;
    double envelopeValue;

    double attackDurationSeconds;
    double releaseDurationSeconds;

    int elapsedSamplesInStage;
    int totalSamplesInStage;
};

#endif // ENVELOPE_H
