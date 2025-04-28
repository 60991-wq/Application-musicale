//
// Created by anini on 26-04-25.
//

#ifndef FILTER_H
#define FILTER_H

#pragma once

#include <cmath>

class Filter {
public:
    Filter();

    void setSampleRate(float rate);
    void setCutoff(float cutoff);
    void setResonance(float resonance);
    void reset();
    void process(float* buffer, int framesPerBuffer);

private:
    void updateCoefficients();

    float sampleRate;
    float cutoff;
    float resonance;

    float a0, a1, a2, b1, b2;
    float x1L, x2L, y1L, y2L;

};

#endif // FILTER_H
