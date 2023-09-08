#pragma once

#include <string>

struct Text {

    // Text string for display
    std::string text;
    std::string font_family{"Cozette"};
    // Whether text should be rendered in screen-space or world-space
    bool gui_element{ false };
};