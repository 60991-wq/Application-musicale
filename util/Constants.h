//
// Created by anini on 10-04-25.
//#pragma once

#include <numbers>

namespace Constants {
    constexpr auto TwoPi = std::numbers::pi_v<float> * 2.0f;
    constexpr int SampleRate = 44100;
    constexpr int FramesPerBuffer = 256;
    constexpr float MinCutoff = 20.0f;
    constexpr float MaxCutoff = SampleRate / 2.0f - 200.0f;

}
