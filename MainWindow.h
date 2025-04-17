
#ifndef TESTINSTRUCT_MAINWINDOW_H
#define TESTINSTRUCT_MAINWINDOW_H

#include <SDL3/SDL.h>

#include "audio/Delay.h"


#include "audio/Oscillator.h"

class MainWindow {
public :
    void init();
    void run();
    Oscillator* oscillator1 =  nullptr;
    Oscillator* oscillator2 =  nullptr;
    Delay* delay = nullptr;
private:
    void draw();
    SDL_Window* window { nullptr };
    SDL_Renderer* renderer { nullptr };
};

#endif //TESTINSTRUCT_MAINWINDOW_H
