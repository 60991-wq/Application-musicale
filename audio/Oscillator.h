
// Created by aninia on 25-04-25.

#pragma once

class Oscillator {
public:
    enum class Waveform {
        SINE,
        SQUARE,
        SAW
    };

    explicit Oscillator();

    void setFrequency(double newFrequencyHz);

    void setWaveform(Waveform newWaveform);

    void setSampleRate(double newSampleRate);

    void resetPhase();


    void process(float *audioBuffer);

private:
    double currentSampleRate;
    double currentFrequencyHz;
    double phaseRadians;
    Waveform waveformType;
};
