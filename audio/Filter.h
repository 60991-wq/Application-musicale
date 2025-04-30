#pragma once
#include <cmath>

class Filter {
public:
    Filter();
    void setSampleRate(float rate);
    void setCutoff(float newCutoff);
    void setResonance(float newResonance);
    void reset();
    void process(float* buffer, int framesPerBuffer);

private:
    void updateCoefficients();
    
    float sampleRate;
    float cutoff;
    float resonance;
    
    // Coefficients du filtre
    float a0, a1, a2, b1, b2;
    
    // Variables d'état (une seule série)
    float x1, x2, y1, y2;
};