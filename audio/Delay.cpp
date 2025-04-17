//
// Created by anini on 03-04-25.
//

#include "Delay.h"
#include <cstring>

Delay::Delay(double sampleRate)
    : sampleRate(sampleRate),
    delayMix(0.5f),
    delayTime(0.5),
    writeIndex(0),
    readIndex(0){

    bufferSize = static_cast<int>(sampleRate * 2.0);
    delayBuffer.resize(bufferSize * 2,0.0f);
    updateReadIndex();

}
void Delay::updateReadIndex() {
    int delaySamples = static_cast<int>(sampleRate * delayTime);
    readIndex = writeIndex - delaySamples;
    if (readIndex < 0) {
        readIndex += bufferSize;
    }
}

void Delay::process(float* buffer, int frames) {
    for (int i = 0; i < frames; ++i) {
        // canaux stéréo
        for (int ch = 0; ch < 2; ++ch) {
            int bufIdx = i * 2 + ch;

            float dry = buffer[bufIdx];  // son actuel
            float delayed = delayBuffer[(readIndex * 2 + ch) % (bufferSize * 2)];

            buffer[bufIdx] += delayed * delayMix;  // mix dans l’output
            delayBuffer[(writeIndex * 2 + ch) % (bufferSize * 2)] = dry;  // stocke

        }

        writeIndex = (writeIndex + 1) % bufferSize;
        readIndex = (readIndex + 1) % bufferSize;
    }
}


