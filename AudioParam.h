//
// Created by anini on 25-04-25.
//

#ifndef AUDIOPARAM_H
#define AUDIOPARAM_H

#include <mutex>

struct POD {
    bool osc1Active = true;
    bool osc2Active = false;

    int osc1Waveform = 0; // 0 = SINE, 1 = SQUARE, 2 = SAW
    float osc1Offset = 0.0f;

    float attack = 0.1f;
    float release = 0.1f;

    float cutoff = 800.0f;
    float resonance = 0.0f;

    float delayTime = 0.3f;
    float delayMix = 0.2f;

    int activeNote = -1; // -1 = aucune note jouée
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
