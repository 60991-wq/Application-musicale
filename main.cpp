#include <iostream>
#include "MainWindow.h"
#include "AudioGenerator.h"
#include "audio/Oscillator.h"
#include <thread>
#include <chrono>


int main() {

    Oscillator oscillator;
    oscillator.setWaveform(Oscillator::Waveform::SINE);
    std::cout << "Forme d'onde sélectionnée : " << static_cast<int>(Oscillator::Waveform::SAW) << std::endl;
    oscillator.setFrequency(440.0f);

    AudioGenerator generator;
    generator.init(&oscillator);
    std::cin.get();
    return 0;

}
