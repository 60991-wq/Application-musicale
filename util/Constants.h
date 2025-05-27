//
// Created by anini on 10-04-25.
//#pragma once

#include <numbers>

namespace Constants {
    constexpr auto TwoPi = std::numbers::pi_v<float> * 2.0f;
    constexpr int SampleRate = 44100;
    constexpr int FramesPerBuffer = 256;
    constexpr int ControlWidth = 600.0f;
    constexpr float MinCutoff = 20.0f;
    constexpr float MaxCutoff = 20000.0f;
}
