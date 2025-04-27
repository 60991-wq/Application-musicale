#ifndef AUDIOGENERATOR_H
#define AUDIOGENERATOR_H

#include "portaudio.h"
#include "audio/Oscillator.h"
#include "audio/Filter.h"
#include "AudioParam.h"
#include <vector>

struct AudioCallbackData {
    LockedPOD* lockedParams;
};

class AudioGenerator {
public:
    explicit AudioGenerator(LockedPOD& sharedParams);
    void init();
    void cleanup();  // Méthode explicite pour le nettoyage

private:
    static int audioCallback(const void*, void* outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags,
                             void* userData);

    PaStream* stream {nullptr};
    AudioCallbackData callbackData;

    Oscillator osc1;
    Oscillator osc2;
    Filter filter;
    Envelope envelope;
    bool noteWasPressed { false };
};

#endif // AUDIOGENERATOR_H