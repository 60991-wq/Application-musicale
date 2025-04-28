#include "Filter.h"
#include "../util/Constants.h"
#include <iostream>
#include <cmath>
#include <algorithm> // Pour std::min, std::max, std::clamp

Filter::Filter()
    : sampleRate(Constants::SampleRate), cutoff(1000.0f), resonance(0.5f),
      a0(1.0f), a1(0.0f), a2(0.0f), b1(0.0f), b2(0.0f),
      x1L(0.0f), x2L(0.0f), y1L(0.0f), y2L(0.0f),
      x1R(0.0f), x2R(0.0f), y1R(0.0f), y2R(0.0f)
{
    updateCoefficients();
}

void Filter::setSampleRate(float rate) {
    sampleRate = rate;
    updateCoefficients();
}

void Filter::setCutoff(float newCutoff) {
    // Limiter la fréquence de coupure à des valeurs raisonnables
    cutoff = std::clamp(newCutoff, 20.0f, sampleRate * 0.49f);
    updateCoefficients();
}

void Filter::setResonance(float newResonance) {
    // Limiter la résonance pour éviter l'instabilité
    resonance = std::clamp(newResonance, 0.01f, 0.99f);
    updateCoefficients();
}

void Filter::reset() {
    x1L = x2L = y1L = y2L = 0.0f;
    x1R = x2R = y1R = y2R = 0.0f;
}

void Filter::updateCoefficients() {
    float safeCutoff = std::clamp(cutoff, 20.0f, sampleRate * 0.49f);
    float safeResonance = std::clamp(resonance, 0.01f, 0.99f);

    // Calculer Q à partir de la résonance (formule standard)
    float q = 0.5f / (1.0f - safeResonance);

    // Calculer omega (fréquence normalisée)
    float omega = 2.0f * Constants::TwoPi * safeCutoff / sampleRate;

    // Limiter omega pour éviter des problèmes numériques
    omega = std::min(omega, 3.1f);

    float sinw = std::sin(omega);
    float cosw = std::cos(omega);
    float alpha = sinw / (2.0f * q);

    // S'assurer que le dénominateur n'est jamais zéro
    float denom = 1.0f + alpha;
    if (denom < 0.00001f) denom = 0.00001f;
    float norm = 1.0f / denom;

    // Calculer les coefficients pour le filtre passe-bas
    a0 = (1.0f - cosw) * 0.5f * norm;
    a1 = (1.0f - cosw) * norm;
    a2 = (1.0f - cosw) * 0.5f * norm;
    b1 = -2.0f * cosw * norm;
    b2 = (1.0f - alpha) * norm;

    // Débogage des coefficients
//     std::cout << "Filtre: cutoff=" << safeCutoff << ", resonance=" << safeResonance
//               << std::endl;
 }
void Filter::process(float* buffer, int framesPerBuffer) {
    for (int i = 0; i < framesPerBuffer; ++i) {
        // Récupérer l'entrée (nous n'utilisons que le canal gauche puisque c'est le même signal)
        float input = buffer[2 * i];

        // Calculer la sortie une seule fois
        float output = a0 * input + a1 * x1L + a2 * x2L - b1 * y1L - b2 * y2L;

        // Limiter la sortie pour éviter les explosions numériques
        output = std::clamp(output, -1.0f, 1.0f);

        // Mise à jour des variables d'état (une seule fois)
        x2L = x1L;
        x1L = input;
        y2L = y1L;
        y1L = output;

        // Appliquer la même sortie aux deux canaux
        buffer[2 * i] = output;     // Canal gauche
        buffer[2 * i + 1] = output; // Canal droit
    }
}