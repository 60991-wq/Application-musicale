#include "MainWindow.h"
#include <thread>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include <cmath>

constexpr float FRAMERATE = 60.0f;
constexpr std::chrono::duration<double, std::milli> TARGET_FRAMETIME(1000.0 / FRAMERATE);

void MainWindow::init() {
    // (Setup SDL + ImGui exactement comme avant)
}

void MainWindow::run() {
    const auto clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool done { false };
    while (!done){
        auto frameStart = std::chrono::high_resolution_clock::now();

        SDL_Event event;
        while (SDL_PollEvent(&event)){
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (SDL_EVENT_QUIT == event.type)
                done = true;
            if ((SDL_EVENT_WINDOW_CLOSE_REQUESTED == event.type)
                && (SDL_GetWindowID(window) == event.window.windowID))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // all the UI code description
        draw();

        // Rendering
        ImGui::Render();
        SDL_SetRenderDrawColorFloat(renderer,
                                    clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);

        // Calculate time spent and sleep if needed
        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto frameDuration = frameEnd - frameStart;
        if (frameDuration < TARGET_FRAMETIME) {
            std::this_thread::sleep_for(TARGET_FRAMETIME - frameDuration);
        }
    }

    // Cleanup
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void MainWindow::draw() {

    ImGui::Begin("Synthétiseur");

    const char* waveforms[] = { "SINE", "SQUARE", "SAW" };

    ImGui::Checkbox("OSC 1", &uiState.osc1Active);
    ImGui::Combo("##Waveform1", &uiState.osc1Waveform, waveforms, IM_ARRAYSIZE(waveforms));
    ImGui::SameLine(); ImGui::Text("OSC1 Waveform");

    ImGui::SliderFloat("##Offset1", &uiState.osc1Offset, -5.0f, 5.0f, "%.3f");
    ImGui::SameLine(); ImGui::Text("OSC1 Frequency Offset");

    ImGui::Checkbox("OSC 2", &uiState.osc2Active);
    ImGui::Text("OSC2 Waveform: SAW (non modifiable)");

    ImGui::SliderFloat("Attack", &uiState.attack, 0.0f, 1.0f);
    ImGui::SliderFloat("Release", &uiState.release, 0.0f, 2.0f);
    ImGui::SliderFloat("Cutoff", &uiState.cutoff, 20.0f, 20000.0f);
    ImGui::SliderFloat("Resonance", &uiState.resonance, 0.0f, 5.0f);

    ImGui::SliderFloat("Delay Time", &uiState.delayTime, 0.1f, 2.0f);
    ImGui::SliderFloat("Delay Mix", &uiState.delayMix, 0.0f, 1.0f);

    ImGui::Separator();
    ImGui::Text("Clavier virtuel");

    for (int i = 0; i < 13; ++i) {
        char label[4];
        sprintf(label, "%d", i + 1);
        if (ImGui::Button(label, ImVec2(36, 36))) {
            uiState.activeNote = i;
            uiState.noteTriggered = true;
        }
        if (i < 12) ImGui::SameLine();
    }

    ImGui::End();
}

