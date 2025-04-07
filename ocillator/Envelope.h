//
// Created by anini on 03-04-25.
//

#ifndef ENVELOPE_H
#define ENVELOPE_H



class Envelope {
public:
    Envelope(double sampleRate);
    void noteOn();
    void noteOff();

    void setAttackTime(double seconds);
    void setReleaseTime(double seconds);
    void process(float* buffer, int frame);

private:
    enum {IDLE, ATTACK, RELEASE}state;
    double sampleRate;
    double envelopeValue;
    double attackIncrement;
    double releaseIncrement;

    double attackTime;
    double releaseTime;

    void updateIncrement();

};



#endif //ENVELOPE_H
