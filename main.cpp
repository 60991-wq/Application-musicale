#include <iostream>
#include "MainWindow.h"
#include "AudioGenerator.h"
#include "audio/Oscillator.h"
#include <thread>
#include <chrono>
#include "util/Constants.h"
#include "audio/Delay.h"

int main() {
    Oscillator osc1(SAMPLE_RATE);
    Oscillator osc2(SAMPLE_RATE);

    osc1.setWaveform(Oscillator::Waveform::SINE);
    osc1.setFrequency(440.0);
    osc1.setEnvelopeParams(0.5, 0.3, 0.6, 1.0);
    osc1.setCutoff(800.0);

    osc2.setWaveform(Oscillator::Waveform::SQUARE);
    osc2.setFrequency(220.0);
    osc2.setEnvelopeParams(0.4, 0.2, 0.7, 1.2);
    osc2.setCutoff(600.0);

    Delay delay(SAMPLE_RATE);
    delay.setDelayTime(0.5);
    delay.setMix(0.4f);

    AudioGenerator generator;
    generator.init(&osc1, &delay); // Par défaut, on écoute osc1. (On pourra mixer ensuite)

    osc1.noteOn();


    MainWindow window;
    window.oscillator1 = &osc1;
    window.oscillator2 = &osc2;
    window.delay = &delay;

    window.init();
    window.run();

    return 0;
}
