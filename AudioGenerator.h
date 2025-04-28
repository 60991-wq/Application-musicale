#ifndef AUDIOGENERATOR_H
#define AUDIOGENERATOR_H

#include "portaudio.h"
#include "audio/Oscillator.h"
#include "audio/Filter.h"
#include "AudioParam.h"
#include <vector>


class AudioGenerator {
public:
    explicit AudioGenerator(LockedPOD& sharedParams);
    void init();
private:
    static int audioCallback(const void*, void* outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags,
                             void* userData);

    PaStream* stream {nullptr};
    LockedPOD& params;
};

#endif // AUDIOGENERATOR_H