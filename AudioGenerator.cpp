#include <iostream>
#include "AudioGenerator.h"
#include "util/Constants.h"
#include <cmath>


void AudioGenerator::init(Oscillator *oscillator) {
this->oscillator = oscillator;

    PaError errorInit = Pa_Initialize();
    if( errorInit != paNoError ) {
        std::cerr << "PortAudio error in Pa_Initialize(): "
                  << Pa_GetErrorText( errorInit ) << std::endl;
        return;
    }

    PaError errorStream;
    PaStream *stream;

    errorStream = Pa_OpenDefaultStream(&stream,
                                       0,
                                       2,
                                       paFloat32,
                                       SAMPLE_RATE,
                                       FRAMES_PER_BUFFER,
                                       audioCallback,
                                       this );

    errorStream = Pa_StartStream( stream );
    if( errorStream != paNoError ) {
        std::cerr << "PortAudio error in Pa_StartStream(): "
                  << Pa_GetErrorText( errorStream ) << std::endl;
        return;
    }
}

int AudioGenerator::audioCallback(const void *inputBuffer,
                                  void *outputBuffer,
                                  unsigned long framesPerBuffer,
                                  const PaStreamCallbackTimeInfo *timeInfo,
                                  PaStreamCallbackFlags statusFlags,
                                  void *userData) {


    auto* generator = static_cast<AudioGenerator*>(userData);
    float* audioBuffer = reinterpret_cast<float*>(outputBuffer);

    // Remplir le buffer avec des zéros pour commencer
    std::fill(audioBuffer, audioBuffer + framesPerBuffer * 2, 0.0f);

    // Générer le son avec ton Oscillator
    if (generator->oscillator) {
        generator->oscillator->generate(audioBuffer, static_cast<int>(framesPerBuffer));
    }

    return paContinue;
}

