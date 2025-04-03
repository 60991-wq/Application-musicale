//
// Created by anini on 03-04-25.
//

#ifndef OSCILLATOR_H
#define OSCILLATOR_H



class Oscillator {
    public:
    enum Waveform {SINE, SQUARE, SAW};
     explicit Oscillator(double sampleRate);

    void setFrequency(double hz);
    void setWaveform(Waveform waveform);
    void  setfrequencyOffset(double offset);
    void generate(float* buffer,int frames);


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
