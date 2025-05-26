#include "Filter.h"
#include <algorithm> // pour std::clamp
#include <iostream>
#include "../util/Constants.h"


Filter::Filter()
    :  cutoff(1000.0f), resonance(0.5f),
a0(1.0f),a1(0.0f),a2(0.0f), b1(0.0f), b2(0.0f),
    x1(0.0f), x2(0.0f), y1(0.0f), y2(0.0f)
       {
     updateCoefficients();
}

void Filter::setCutoff(float newCutoff) {
    cutoff = std::clamp(newCutoff, 50.0f, Constants::FilterCutoff-1);
    updateCoefficients();
}

void Filter::setResonance(float newResonance) {
    resonance = std::clamp(newResonance, 0.1f, 0.9f);
    updateCoefficients();
}

void Filter::updateCoefficients() {
    cutoff = std::clamp(cutoff, 20.0f,Constants::FilterCutoff-100.0f);
    resonance = std::clamp(resonance, 0.01f, 0.99f);
    float q = 0.5f / (1.0f - resonance);
    float omega = 2.0f * Constants::TwoPi * cutoff / Constants::SampleRate;
    float alpha = std::sin(omega) / (2.0f * q);
    float cosw = std::cos(omega);
    float norm = 1.0f / (1.0f + alpha);
    a0 = (1.0f - cosw) * 0.5f * norm;
    a1 = (1.0f - cosw) * norm;
    a2 = (1.0f - cosw) * 0.5f * norm;
    b1 = -2.0f * cosw * norm;
    b2 = (1.0f - alpha) * norm;
}

void Filter::process(float* buffer) {
    for (int i = 0; i < Constants::FramesPerBuffer; ++i) {
        float input = buffer[i];
        float output = (a0 * input) + (a1 * x1) + (a2 * x2) - (b1 * y1) - (b2 * y2);

        buffer[i] = output;

        x2 = x1;
        x1 = input;
        y2 = y1;
        y1 = output;



    }
    static int debugCounter = 0;
    if (debugCounter++ % 1000 == 0) {
        std::cout << "Cutoff: " << cutoff << ", Res: " << resonance << std::endl;
        std::cout << "Coeffs: a0=" << a0 << ", a1=" << a1 << ", a2=" << a2
                  << ", b1=" << b1 << ", b2=" << b2 <<  std::endl;
        std::cout << "Coeffs: y1=" << y1 << ", x1=" << x1  << std::endl;


    }
}