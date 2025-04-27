#pragma once
#include "Envelope.h"
#include "Filter.h"

class Oscillator {
public:
    enum class Waveform {
        SINE,
        SQUARE,
        SAW
    };

    explicit Oscillator(double sampleRate, float noteFreq);
    
    void setFrequency(double hz);
    void setWaveform(Waveform wf);
    void setFrequencyOffset(double offset);
    void setSampleRate(double newSampleRate);
    
    void noteOn();
    void noteOff();
    void setEnvelopeParams(double attack, double release);
    void setCutoff(double cutoffHz);
    
    // Remplacer generate par process
    void process(float* buffer, int frames);

private:
    void updatePhaseStep();
    
    double sampleRate;
    double frequency;
    double frequencyOffset;
    double phase;
    double phaseStep;
    Waveform waveform;
    
    Filter filter;
};