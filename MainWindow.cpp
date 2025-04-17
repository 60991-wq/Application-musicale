
#include "MainWindow.h"
#include <thread>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include <cmath>

constexpr float FRAMERATE = 60.0f;
constexpr std::chrono::duration<double, std::milli> TARGET_FRAMETIME(1000.0 / FRAMERATE);

void MainWindow::init() {

    // Setup SDL
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        SDL_Log("Error: SDL_Init(): %s", SDL_GetError());
        return;
    }
    // Create window with SDL_Renderer graphics context
    Uint32 window_flags = SDL_WINDOW_HIDDEN;
    window = SDL_CreateWindow("", 800, 500, window_flags);
    if (nullptr == window) {
        SDL_Log("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return;
    }
    renderer = SDL_CreateRenderer(window, nullptr);
    SDL_SetRenderVSync(renderer, 1);
    if (nullptr == renderer) {
        SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
        return;
    }
    SDL_SetWindowPosition(
            window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // Setup DearImGui style
    ImGui::StyleColorsLight();
    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 4.0f;
    style.FramePadding = ImVec2(6, 4);
    style.ItemSpacing = ImVec2(10, 6);

    ImGui::GetStyle().WindowRounding = 0.0f;

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
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

    static bool osc1_enabled = true;
    static bool osc2_enabled = false;

    static int waveform1 = 0, waveform2 = 0;
    static float offset1 = 0.0f, offset2 = 0.0f;

    static float attack = 0.5f, release = 0.5f;
    static float cutoff = 1000.0f, resonance = 0.5f;
    static float delayTime = 0.4f, delayMix = 0.5f;

    const char* waveforms[] = { "SINE", "SQUARE", "SAW" };

    // ============ OSC 1 ============
    ImGui::Checkbox("OSC 1", &osc1_enabled);
    ImGui::Combo("##Waveform1", &waveform1, waveforms, IM_ARRAYSIZE(waveforms));
    ImGui::SameLine(); ImGui::Text("OSC1 Waveform");

    ImGui::SliderFloat("##Offset1", &offset1, -10.0f, 10.0f, "%.3f");
    ImGui::SameLine(); ImGui::Text("OSC1 Frequency Offset");

    // ============ OSC 2 ============
    ImGui::Checkbox("OSC 2", &osc2_enabled);
    ImGui::Combo("##Waveform2", &waveform2, waveforms, IM_ARRAYSIZE(waveforms));
    ImGui::SameLine(); ImGui::Text("OSC2 Waveform");

    ImGui::SliderFloat("##Offset2", &offset2, -10.0f, 10.0f, "%.3f");
    ImGui::SameLine(); ImGui::Text("OSC2 Frequency Offset");

    // ============ ENVELOPE & EFFETS ============
    ImGui::SliderFloat("##Attack", &attack, 0.0f, 2.0f, "%.3f");
    ImGui::SameLine(); ImGui::Text("Attack");

    ImGui::SliderFloat("##Release", &release, 0.0f, 3.0f, "%.3f");
    ImGui::SameLine(); ImGui::Text("Release");

    ImGui::SliderFloat("##Cutoff", &cutoff, 20.0f, 5000.0f, "%.1f");
    ImGui::SameLine(); ImGui::Text("Filter Cutoff");

    ImGui::SliderFloat("##Resonance", &resonance, 0.0f, 5.0f, "%.3f");
    ImGui::SameLine(); ImGui::Text("Filter Resonance");

    ImGui::SliderFloat("##DelayTime", &delayTime, 0.0f, 2.0f, "%.3f");
    ImGui::SameLine(); ImGui::Text("Delay Time");

    ImGui::SliderFloat("##DelayMix", &delayMix, 0.0f, 1.0f, "%.3f");
    ImGui::SameLine(); ImGui::Text("Delay Mix");

    // Appliquer les paramètres
    if (osc1_enabled && oscillator1) {
        oscillator1->setWaveform(static_cast<Oscillator::Waveform>(waveform1));
        oscillator1->setEnvelopeParams(attack, 0.3f, 0.6f, release);
        oscillator1->setCutoff(cutoff);
        oscillator1->setFrequencyOffset(offset1);
    }

    if (osc2_enabled && oscillator2) {
        oscillator2->setWaveform(static_cast<Oscillator::Waveform>(waveform2));
        oscillator2->setEnvelopeParams(attack, 0.3f, 0.6f, release);
        oscillator2->setCutoff(cutoff);
        oscillator2->setFrequencyOffset(offset2);
    }

    if (delay) {
        delay->setDelayTime(delayTime);
        delay->setMix(delayMix);
    }

    // ============ CLAVIER ============
    ImGui::Separator();
    ImGui::Text("Clavier virtuel");
    for (int i = 0; i < 13; ++i) {
        char label[4];
        sprintf(label, "%d", i + 1);
        if (ImGui::Button(label, ImVec2(36, 36))) {
            float freq = 220.0f * std::pow(2.0f, i / 12.0f);
            if (osc1_enabled && oscillator1) oscillator1->setFrequency(freq + offset1);
            if (osc2_enabled && oscillator2) oscillator2->setFrequency(freq + offset2);
            if (osc1_enabled && oscillator1) oscillator1->noteOn();
            if (osc2_enabled && oscillator2) oscillator2->noteOn();
        }
        if (i < 12) ImGui::SameLine();
    }

    ImGui::End();
}
