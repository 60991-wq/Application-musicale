#include <iostream>
#include "MainWindow.h"
#include "AudioGenerator.h"
#include "audio/Oscillator.h"
#include <thread>
#include <chrono>


int main() {

    Oscillator oscillator;
    oscillator.setWaveform(Oscillator::Waveform::SINE);
    //std::cout << "Forme d'onde sélectionnée : " << static_cast<int>(Oscillator::Waveform::SQUARE) << std::endl;
    oscillator.setFrequency(440.0f);

    oscillator.setEnvelopeParams(
        0.5,  // Attack time en secondes
        0.3,  // Decay time
        0.6,  // Sustain level (entre 0.0 et 1.0)
        1.0   // Release time
    );

    AudioGenerator generator;
    generator.init(&oscillator);

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
