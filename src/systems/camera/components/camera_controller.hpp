#pragma once

#include <vector>
#include <string>

struct CameraController {
    float buffer = 5;
    CameraController() {}
    CameraController(std::vector<std::string> args) {}
};