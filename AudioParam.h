// Created by anini on 25-04-25.
//

#ifndef AUDIOPARAM_H
#define AUDIOPARAM_H

#include <mutex>

struct SynthParameters {
    // tout ce qui est de ocillator 1
    bool osc1Active = true;
    int osc1Waveform = 0; // 0 = SINE, 1 = SQUARE, 2 = SAW
    float osc1phase = 0.0f;
    float osc1OFrequencyOffset = 0.0f;

    // tout ce qui concerne oscillator 2
    bool osc2Active = true;
    int osc2Waveform = 2;
    float osc2phase = 0.0f;

    // les attribut de l'envelope
    float attack = 0.1;
    float release = 0.5f;

    // les attribut de filter
    float cutoff = 15000.0f;
    float resonance = 0.5f;
    float delayTime = 0.1f;
    float delayMix = 0.0f;

    bool activeNote = false; // -1 = aucune note jouée
    int noteIndex = 0;
};

class LockedSynthParameters {
public:
    LockedSynthParameters() = default;
    SynthParameters getCopy() const;
    void setCopy(const SynthParameters& newData);

private:
    mutable std::mutex mutex;
    SynthParameters data;
};

#endif // AUDIOPARAM_H