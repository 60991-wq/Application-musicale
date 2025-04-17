#include <iostream>
#include "MainWindow.h"
#include "AudioGenerator.h"
#include "audio/Oscillator.h"
#include <thread>
#include <chrono>
#include "util/Constants.h"
#include "audio/Delay.h"


int main() {

    Oscillator oscillator;
    oscillator.setWaveform(Oscillator::Waveform::SAW);
    //std::cout << "Forme d'onde sélectionnée : " << static_cast<int>(Oscillator::Waveform::SQUARE) << std::endl;
    oscillator.setFrequency(440.0f);

    oscillator.setEnvelopeParams(
        0.5,  // Attack time en secondes
        0.3,  // Decay time
        0.6,  // Sustain level (entre 0.0 et 1.0)
        1.0   // Release time
    );
    oscillator.setCutoff(800.0);


    Delay delay(SAMPLE_RATE);
    delay.setDelayTime(0.5);  // 500 ms
    delay.setMix(0.4f);

    AudioGenerator generator;
    generator.init(&oscillator, &delay);

    oscillator.noteOn();
    std::cout << "Note ON (attack + decay + sustain phase)" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));


    oscillator.noteOff();
    std::cout << "Note OFF (release phase)" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "Appuyez sur Entrée pour quitter..." << std::endl;

    std::cin.get();







    return 0;

}
