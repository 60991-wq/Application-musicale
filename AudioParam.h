//
// Created by anini on 25-04-25.
//

#ifndef AUDIOPARAM_H
#define AUDIOPARAM_H

#include <mutex>

struct POD {
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
    float attack = 0.0;
    float release = 0.0f;

    // les attribut de filter
    float cutoff = 15000.0f;
    float resonance = 0.4f;
    float delayTime = 0.0f;
    float delayMix = 0.0f;

    bool activeNote = false; // -1 = aucune note jouée
    int noteIndex = 0;
};

class LockedPOD {
public:
    LockedPOD() = default;
    POD getCopy() const;
    void setCopy(const POD& newData);

private:
    mutable std::mutex mutex;
    POD data;
};

#endif // AUDIOPARAM_H
