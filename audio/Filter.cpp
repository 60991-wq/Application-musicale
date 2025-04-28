#include "Filter.h"
#include "../util/Constants.h"

Filter::Filter()
    : sampleRate(Constants::SampleRate), cutoff(20000.0f), resonance(0.0f),
      a0(1.0f), a1(0.0f), a2(0.0f), b1(0.0f), b2(0.0f),
      x1L(0.0f), x2L(0.0f), y1L(0.0f), y2L(0.0f)
{
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
    x1L = x2L = y1L = y2L = 0.0f;
    x1R = x2R = y1R = y2R = 0.0f;

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
        // Canal gauche
        float inputL = buffer[2 * i];
        float outputL = a0 * inputL + a1 * x1L + a2 * x2L - b1 * y1L - b2 * y2L;
        x2L = x1L;
        x1L = inputL;
        y2L = y1L;
        y1L = outputL;
        buffer[2 * i] = outputL;
        float inputR = buffer[2 * i + 1];
        float outputR = a0 * inputR + a1 * x1R + a2 * x2R - b1 * y1R - b2 * y2R;
        x2R = x1R;
        x1R = inputR;
        y2R = y1R;
        y1R = outputR;
        buffer[2 * i + 1] = outputR;

    }
}
