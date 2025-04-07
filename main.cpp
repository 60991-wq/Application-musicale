#include <iostream>
#include "MainWindow.h"
#include "AudioGenerator.h"
#include "ocillator/Oscillator.h"
// Constantes
constexpr double SAMPLE_RATE = 44100.0;
constexpr unsigned long FRAMES_PER_BUFFER = 256;

Oscillator osc(SAMPLE_RATE);

// Callback audio appelée automatiquement par PortAudio
static int audioCallback(const void *input,
                         void *output,
                         unsigned long frameCount,
                         const PaStreamCallbackTimeInfo *,
                         PaStreamCallbackFlags,
                         void *userData) {

    float *out = static_cast<float *>(output);
    std::fill(out, out + frameCount * 2, 0.0f);  // on vide le buffer d'abord (stéréo = *2)

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
    osc.setWaveform(Oscillator::SINE);  // Tu peux tester SQUARE ou SAW aussi
    osc.setFrequency(440.0);            // La4
    osc.setFrequencyOffset(2.0);        // Décalage subtil

    // Initialisation PortAudio
    err = Pa_Initialize();
    if (err != paNoError) goto error;

    PaStream *stream;
    err = Pa_OpenDefaultStream(&stream,
                               0,          // pas d'entrée
                               2,          // sortie stéréo
                               paFloat32,  // format
                               SAMPLE_RATE,
                               FRAMES_PER_BUFFER,
                               audioCallback,
                               &osc);
    if (err != paNoError) goto error;

    err = Pa_StartStream(stream);
    if (err != paNoError) goto error;

    std::cout << "Oscillateur en cours (La4, offset +2Hz)... Appuie sur Entrée pour quitter." << std::endl;
    std::cin.get();

    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();
    return 0;

    error:
        std::cerr << "Erreur PortAudio : " << Pa_GetErrorText(err) << std::endl;
    return 1;








}
