#include "Filter.h"
#include <cmath> // pour M_PI
#include <algorithm> // pour std::min

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Filter::Filter(double sampleRate)
    : sampleRate(sampleRate), cutoff(1000.0), resonance(0.0), lastOutput(0.0f) {
    updateAlpha();
}

void Filter::setCutoff(double cutoffHz) {
    cutoff = cutoffHz;
    updateAlpha();
}

void Filter::setResonance(double res) {
    resonance = res;
    updateAlpha();
}

void Filter::reset() {
    lastOutput = 0.0f;
}

float Filter::process(float input) {
    lastOutput = static_cast<float>(alpha * input + (1.0 - alpha) * lastOutput);
    return lastOutput;
}

void Filter::updateAlpha() {
    // Version simple du filtre passe-bas avec résonance
    double q = 0.5 / (1.0 - std::min(resonance, 0.99));
    double rc = 1.0 / (2.0 * M_PI * cutoff);
    double dt = 1.0 / sampleRate;
    alpha = dt / (rc + dt * q);
}