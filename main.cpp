#include "MainWindow.h"
#include "AudioGenerator.h"
#include "AudioParam.h" // Attention ! Toi c'est AudioParam.h (pas AudioParams.h comme ton pote)

int main() {
    LockedPOD sharedParams;

    AudioGenerator audioGenerator(sharedParams);
    audioGenerator.init();

    MainWindow mainWindow(sharedParams);
    mainWindow.init();
    mainWindow.run();
    audioGenerator.cleanup();

    return 0;
}
