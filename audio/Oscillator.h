#pragma once
#include "Envelope.h"
#include "Filter.h"

class Oscillator {
public: // tout les forme d'onde que je dois avoir
    enum class Waveform {
        SINE,
        SQUARE,
        SAW
    };

    explicit Oscillator(double sampleRate, float noteFreq);
    
    void setFrequency(double hz);
    void setWaveform(Waveform wf);
    void setSampleRate(double rate);

    void process(float* buffer, int frames);


private:
    double sampleRate;
    double frequency;
    double phase;
    Waveform waveform;
};