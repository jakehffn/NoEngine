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

class TextSystem : public System {

public:
    TextSystem(entt::registry& registry);

    void update() override;

private:
    struct FontCharacter {
        AtlasData frame_data;
        glm::vec2 bearing; // The offset from the baseline to the left/top of glyph
        float advance; // Offset to the next glyph
    };

    struct FontMap {
        FontMap() : characters(256) {};
        std::vector<FontCharacter> characters;
    };
    
    std::unordered_map<std::string, FontMap> fonts;
};