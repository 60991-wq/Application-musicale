#include "gui/MainWindow.h"
#include "audio/AudioGenerator.h"
#include "audio/AudioParam.h"
#include <iostream>

int main() {
    LockedSynthParameters sharedParams;

    AudioGenerator generator(sharedParams);
    generator.init();

    MainWindow window(sharedParams);
    window.init();
    window.run();

    std::cout << "Application terminée." << std::endl;
    return 0;
}
