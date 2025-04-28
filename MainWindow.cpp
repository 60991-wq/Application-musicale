// #include "MainWindow.h"
// #include <thread>
// #include "imgui.h"
// #include "imgui_impl_sdl3.h"
// #include "imgui_impl_sdlrenderer3.h"
// #include <cmath>
//
// constexpr float FRAMERATE = 60.0f;
// constexpr std::chrono::duration<double, std::milli> TARGET_FRAMETIME(1000.0 / FRAMERATE);
//
// MainWindow::MainWindow(LockedPOD& params)
//     : params(params) // Initialisation du LockedPOD
// {}
// void MainWindow::init() {
//
//     // Setup SDL
//     if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
//         SDL_Log("Error: SDL_Init(): %s\n", SDL_GetError());
//         return;
//     }
//     // Create window with SDL_Renderer graphics context
//     Uint32 window_flags = SDL_WINDOW_HIDDEN;
//     window = SDL_CreateWindow("", 800, 600, window_flags);
//     if (nullptr == window) {
//         SDL_Log("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
//         return;
//     }
//     renderer = SDL_CreateRenderer(window, nullptr);
//     SDL_SetRenderVSync(renderer, 1);
//     if (nullptr == renderer) {
//         SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
//         return;
//     }
//     SDL_SetWindowPosition(
//             window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
//     SDL_ShowWindow(window);
//
//     // Setup Dear ImGui context
//     IMGUI_CHECKVERSION();
//     ImGui::CreateContext();
//     ImGuiIO& io = ImGui::GetIO(); (void)io;
//     io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
//     io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
//
//     // Setup DearImGui style
//     ImGui::StyleColorsDark();
//     ImGui::GetStyle().WindowRounding = 0.0f;
//
//     // Setup Platform/Renderer backends
//     ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
//     ImGui_ImplSDLRenderer3_Init(renderer);
// }
//
// void MainWindow::run() {
//     const auto clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
//
//     bool done { false };
//     while (!done){
//         auto frameStart = std::chrono::high_resolution_clock::now();
//
//         SDL_Event event;
//         while (SDL_PollEvent(&event)){
//             ImGui_ImplSDL3_ProcessEvent(&event);
//             if (SDL_EVENT_QUIT == event.type)
//                 done = true;
//             if ((SDL_EVENT_WINDOW_CLOSE_REQUESTED == event.type)
//                 && (SDL_GetWindowID(window) == event.window.windowID))
//                 done = true;
//         }
//
//         // Start the Dear ImGui frame
//         ImGui_ImplSDLRenderer3_NewFrame();
//         ImGui_ImplSDL3_NewFrame();
//         ImGui::NewFrame();
//
//         // all the UI code description
//         draw();
//
//         // Rendering
//         ImGui::Render();
//         SDL_SetRenderDrawColorFloat(renderer,
//                                     clear_color.x, clear_color.y, clear_color.z, clear_color.w);
//         SDL_RenderClear(renderer);
//         ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
//         SDL_RenderPresent(renderer);
//
//         // Calculate time spent and sleep if needed
//         auto frameEnd = std::chrono::high_resolution_clock::now();
//         auto frameDuration = frameEnd - frameStart;
//         if (frameDuration < TARGET_FRAMETIME) {
//             std::this_thread::sleep_for(TARGET_FRAMETIME - frameDuration);
//         }
//     }
//
//     // Cleanup
//     ImGui_ImplSDLRenderer3_Shutdown();
//     ImGui_ImplSDL3_Shutdown();
//     ImGui::DestroyContext();
//
//     SDL_DestroyRenderer(renderer);
//     SDL_DestroyWindow(window);
//     SDL_Quit();
// }
// void MainWindow::draw() {
//     ImGui::Begin("Synthétiseur");
//
//     POD currentState = params.getCopy();
//
//     const char* waveforms[] = { "SINE", "SQUARE", "SAW" };
//
//     ImGui::Checkbox("OSC 1", &currentState.osc1Active);
//     ImGui::Combo("##Waveform1", &currentState.osc1Waveform, waveforms, IM_ARRAYSIZE(waveforms));
//     ImGui::SameLine(); ImGui::Text("OSC1 Waveform");
//
//     ImGui::SliderFloat("##Offset1", &currentState.osc1Offset, -5.0f, 5.0f, "%.3f");
//     ImGui::SameLine(); ImGui::Text("OSC1 Frequency Offset");
//
//     ImGui::Checkbox("OSC 2", &currentState.osc2Active);
//     ImGui::Text("OSC2 Waveform: SAW (non modifiable)");
//
//     ImGui::SliderFloat("Attack", &currentState.attack, 0.0f, 1.0f);
//     ImGui::SliderFloat("Release", &currentState.release, 0.0f, 2.0f);
//     ImGui::SliderFloat("Cutoff", &currentState.cutoff, 20.0f, 20000.0f);
//     ImGui::SliderFloat("Resonance", &currentState.resonance, 0.0f, 5.0f);
//
//     ImGui::SliderFloat("Delay Time", &currentState.delayTime, 0.1f, 2.0f);
//     ImGui::SliderFloat("Delay Mix", &currentState.delayMix, 0.0f, 1.0f);
//
//     ImGui::Separator();
//     ImGui::Text("Clavier virtuel");
//
//     static bool anyKeyPressed = false;
//     bool keyPressedThisFrame = false;
//
//     for (int i = 0; i < 13; ++i) {
//         char label[4];
//         sprintf(label, "%d", i + 1);
//
//         if (ImGui::Button(label, ImVec2(36, 36))) {
//             currentState.activeNote = i;
//             keyPressedThisFrame = true;
//         }
//
//         if (i < 12) ImGui::SameLine();
//     }
//
//     // --> Détecter s'il n'y a aucun bouton cliqué
//     if (!keyPressedThisFrame && anyKeyPressed) {
//         currentState.activeNote = -1;
//     }
//
//     anyKeyPressed = keyPressedThisFrame;
//
//     ImGui::End();
//
//     params.setCopy(currentState);
// }
