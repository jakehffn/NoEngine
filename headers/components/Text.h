#pragma once

#include <string>

struct Text {

    // Text string for display
    std::string text;
    // Whether text should be rendered in screen-space or world-space
    bool guiElement{ false };
};