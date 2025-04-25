#ifndef SIMPLE_SYNTH_AUDIOGENERATOR_H
#define SIMPLE_SYNTH_AUDIOGENERATOR_H

#include "portaudio.h"
#include "audio/Oscillator.h"
#include "audio/Envelope.h"
#include "AudioParam.h"

struct AudioCallbackData {
    LockedPOD* lockedParams = nullptr;
};

class AudioGenerator {
public:
    explicit AudioGenerator(LockedPOD& sharedParams);

    void init(); // Lance PortAudio avec notre lockedParams

private:
    AudioCallbackData callbackData;
    PaStream* stream = nullptr;

    static int audioCallback(const void* inputBuffer, void* outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo* timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void* userData);
};

#endif // SIMPLE_SYNTH_AUDIOGENERATOR_H
