//
// Created by anini on 03-04-25.
//
#include "Envelope.h"
#ifndef OSCILLATOR_H
#define OSCILLATOR_H



class Oscillator {
    public:
    enum class Waveform {SINE, SQUARE, SAW}; // je rajoute classe pour eviter les conversion
     explicit Oscillator();

    void setFrequency(double hz);
    void setWaveform(Waveform waveform);
    void setFrequencyOffset(double offset);
    void generate(float* buffer,int frames);

    // ici c'est les methode pour l'envelope
   // void noteOn();
    //void noteOff();



private:
    double phase;
    double phaseStep;
    double frequency;
    double sampleRate;
    double frequencyOffset;
    Waveform waveform;


    void updatePhaseStep();



};

#endif //OSCILLATOR_H
