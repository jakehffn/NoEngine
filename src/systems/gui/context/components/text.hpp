#pragma once

#include <string>
#include <vector>

#include <entt/entt.hpp>

struct Text {
    // Text string for display
    std::u32string text;
    std::string font_family{"Cozette"};
    std::vector<entt::entity> glyphs;
};