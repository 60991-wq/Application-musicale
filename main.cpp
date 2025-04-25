#include "MainWindow.h"
#include "AudioGenerator.h"
#include "audio/Oscillator.h"
#include "audio/Envelope.h"
#include "audio/Filter.h"
#include "audio/Delay.h"
#include "util/Constants.h"
#include <chrono>
#include <thread>

constexpr float FRAMERATE = 60.0f;
constexpr std::chrono::duration<double, std::milli> FRAME_DURATION(1000.0 / FRAMERATE);

int main() {
    Oscillator osc1(SAMPLE_RATE);
    Oscillator osc2(SAMPLE_RATE);
    Envelope envelope(SAMPLE_RATE);
    Filter filter(SAMPLE_RATE);
    Delay delay(SAMPLE_RATE);

    AudioGenerator audioGen;
    audioGen.init(&osc1, &osc2, &envelope, &filter, &delay);

    MainWindow window;
    window.init();

    while (true) {
        auto frameStart = std::chrono::high_resolution_clock::now();

        if (!window.pollEvents())
            break;

        // 🔥 C’est ici qu’on connecte l’audio à l’UI
        audioGen.updateFromUi(window.getUiState());

        window.renderFrame();

        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto frameTime = frameEnd - frameStart;
        if (frameTime < FRAME_DURATION)
            std::this_thread::sleep_for(FRAME_DURATION - frameTime);
    }

    return 0;
}
