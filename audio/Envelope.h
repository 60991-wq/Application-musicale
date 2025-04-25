//
// Created by anini on 03-04-25.
//

#ifndef ENVELOPE_H
#define ENVELOPE_H



class Envelope {
public:
    explicit Envelope(double sampleRate);

    void noteOn();
    void noteOff();

    void setAttackTime(double seconds);
    void setReleaseTime(double seconds);

    double getValue() const;// le volume est entre 0.0 et 1.0
    void update();

private:
    enum  State {IDLE, ATTACK, SUSTAIN,  RELEASE} state;
    double sampleRate;
    double envelopeValue;

    double attackTime;
    double releaseTime;

    double attackIncrement;
    double releaseIncrement;
    void updateIncrements();

};

#endif //ENVELOPE_H
