#include <iostream>
#include "MainWindow.h"
#include "AudioGenerator.h"
#include "ocillator/Oscillator.h"
// Constantes
constexpr double SAMPLE_RATE = 44100.0;
constexpr unsigned long FRAMES_PER_BUFFER = 256;

Oscillator osc(SAMPLE_RATE);

static int audioCallback(const void *input,
                         void *output,
                         unsigned long frameCount,
                         const PaStreamCallbackTimeInfo *,
                         PaStreamCallbackFlags,
                         void *userData) {

    float *out = static_cast<float *>(output);
    std::fill(out, out + frameCount * 2, 0.0f);

    Oscillator *osc = static_cast<Oscillator *>(userData);
    osc->generate(out, static_cast<int>(frameCount));

    return paContinue;
}

int main() {
    //MainWindow mainWindow;
   // AudioGenerator audioGenerator;
    //mainWindow.init();
   // mainWindow.run();
    //audioGenerator.init();

    PaError err;

    // Configuration de l'oscillateur
    osc.setWaveform(Oscillator::SINE);
    osc.setFrequency(440.0);
    osc.setFrequencyOffset(0.0);

    // Initialisation PortAudio
    err = Pa_Initialize();
    if (err != paNoError) goto error;

    PaStream *stream;
    err = Pa_OpenDefaultStream(&stream,
                               0,
                               2,
                               paFloat32,
                               SAMPLE_RATE,
                               FRAMES_PER_BUFFER,
                               audioCallback,
                               &osc);
    if (err != paNoError) goto error;

    err = Pa_StartStream(stream);
    if (err != paNoError) goto error;

    std::cin.get();

    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();
    return 0;

    error:
        std::cerr << "Erreur PortAudio : " << Pa_GetErrorText(err) << std::endl;
    return 1;








}
