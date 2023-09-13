#pragma once

#include <vector>
#include <string>

struct PlayerControl {
    float pixels_per_second = 80.0;

    PlayerControl() {}
    PlayerControl(std::vector<std::string> args) {}
};