#include "Filter.h"
#include <algorithm> // pour std::clamp

#include "../util/Constants.h"


Filter::Filter()
    : sampleRate(44100.0f), cutoff(1000.0f), resonance(0.5f),
      a0(1.0f), a1(0.0f), a2(0.0f), b1(0.0f), b2(0.0f),
      x1(0.0f), x2(0.0f), y1(0.0f), y2(0.0f) {
    updateCoefficients();
}

void Filter::setSampleRate(float rate) {
    sampleRate = rate;
    updateCoefficients();
}

void Filter::setCutoff(float newCutoff) {
    cutoff = newCutoff;
    updateCoefficients();
}

void Filter::setResonance(float newResonance) {
    resonance = newResonance;
    updateCoefficients();
}

void Filter::reset() {
    x1 = x2 = y1 = y2 = 0.0f;
}

void Filter::updateCoefficients() {
    float q = 0.5f / (1.0f - resonance);
    float omega = 2.0f * Constants::TwoPi * cutoff / sampleRate;
    float alpha = std::sin(omega) / (2.0f * q);
    float cosw = std::cos(omega);
    float norm = 1.0f / (1.0f + alpha);

    a0 = (1.0f - cosw) * 0.5f * norm;
    a1 = (1.0f - cosw) * norm;
    a2 = (1.0f - cosw) * 0.5f * norm;
    b1 = -2.0f * cosw * norm;
    b2 = (1.0f - alpha) * norm;
}

void Filter::process(float* buffer, int framesPerBuffer) {
    for (int i = 0; i < framesPerBuffer; ++i) {
        float x = buffer[i];
        float y = a0 * x + a1 * x1 + a2 * x2 - b1 * y1 - b2 * y2;

        y = std::clamp(y, -1.0f, 1.0f);

        buffer[i] = y;

        x2 = x1;
        x1 = x;
        y2 = y1;
        y1 = y;
    }
}