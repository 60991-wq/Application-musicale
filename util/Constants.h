//
// Created by anini on 10-04-25.
//#pragma once

#include <numbers>

namespace Constants {
    constexpr float TwoPi = std::numbers::pi_v<float> * 2.0f;
    constexpr int SampleRate = 44100;
    constexpr int FramesPerBuffer = 256;
}
