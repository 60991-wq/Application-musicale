
#ifndef TESTINSTRUCT_MAINWINDOW_H
#define TESTINSTRUCT_MAINWINDOW_H

#include <SDL3/SDL.h>

#include "AudioParams.h"

class MainWindow {
public :
    void init();
    void run();
  AudioParams& getUiState();
    void draw();
    bool pollEvents();           // ← Gère les événements SDL (retourne true si on continue)
    void renderFrame();
private:
    SDL_Window* window { nullptr };
    SDL_Renderer* renderer { nullptr };
    AudioParams uiState;
};

#endif //TESTINSTRUCT_MAINWINDOW_H
