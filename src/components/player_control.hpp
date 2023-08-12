#pragma once

#include <vector>
#include <string>

struct PlayerControl {
    float pixels_per_millisecond = 80.0/1000.0;

    PlayerControl() {}
    PlayerControl(std::vector<std::string> args) {}
};