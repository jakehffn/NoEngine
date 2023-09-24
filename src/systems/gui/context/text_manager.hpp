#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <cmath>

#include <entt/entt.hpp>
#include <glm/glm.hpp>

// GLEW must come before OpenGL
#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <ft2build.h>
#include FT_FREETYPE_H  

#include "system.hpp"
#include "animation_structs.hpp"

#include "texture_atlas.hpp"

#include "spacial.hpp"
#include "text.hpp"
#include "renderable.hpp"
#include "name.hpp"
#include "gui_element.hpp"
#include "component_grid_ignore.hpp"
#include "persistent.hpp"
#include "dialog_child.hpp"
#include "destroy.hpp"
// Theres an outline conflict in freetype
namespace Component {
    #include "outline.hpp"
}

#include "debug_timer.hpp"

class TextManager  {

public:
    TextManager(entt::registry& registry);

    void update();

    void loadFont(std::string font_path, std::string font_name);
    std::vector<std::u32string> layout(const std::u32string& text, std::string font, float width);
private:
    struct FontCharacter {
        AtlasData* frame_data;
        glm::ivec2 bearing; // The offset from the baseline to the left/top of glyph
        float advance; // Offset to the next glyph
    };

    struct FontMap {
        FontMap() {};
        std::unordered_map<int, FontCharacter> characters;
    };

    std::vector<unsigned char> bitmapToRGBA(unsigned char* data, int width, int height, int pitch);
    void emplaceGlyphs(entt::registry& registry, entt::entity entity);
    void updateGlyphs(entt::registry& registry, entt::entity entity);
    void destroyGlyphs(entt::registry& registry, entt::entity entity);
    
    entt::observer spacial_observer;

    std::unordered_map<std::string, FontMap> fonts;
    entt::registry& registry;
};