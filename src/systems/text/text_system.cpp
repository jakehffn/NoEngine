#include "text_system.hpp"

#include <iostream>

TextSystem::TextSystem(entt::registry& registry) : System(registry) {
    
    this->loadFont("./assets/fonts/cozette/cozette.bdf");

    registry.on_construct<Text>().connect<TextSystem::emplaceTextures>(this);
}

void TextSystem::update() {}

void TextSystem::loadFont(std::string font_path) {
    TextureAtlas& texture_atlas = this->registry.ctx().at<TextureAtlas&>();

    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        #ifndef NDEBUG
            std::cerr << "ERROR: Could not initialize FreeType" << std::endl;
        #endif
    }

    FT_Face face;
    if (FT_New_Face(ft, font_path.c_str(), 0, &face)) {
        #ifndef NDEBUG
            std::cerr << "ERROR: FreeType failed to load font: " << font_path << std::endl;  
        #endif
    }

    FT_Set_Pixel_Sizes(face, 0, face->available_sizes[0].height);  

    int num_glyphs_to_load{256};

    FontMap& font_map{fonts[face->family_name]};
    font_map.characters.resize(num_glyphs_to_load);
    
    for (int c{0}; c < num_glyphs_to_load; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {   
            #ifndef NDEBUG
                std::cout << "ERROR FREETYTPE Failed to load Glyph" << std::endl;
            #endif
            continue;
        }

        std::vector<unsigned char> pixels{this->bitmapToRGBA(
            face->glyph->bitmap.buffer,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            face->glyph->bitmap.pitch
        )};

        TextureSource glyph_texture {
            pixels.data(),
            GL_RGBA,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(0,0),
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(0,0)
        };

        font_map.characters[c] = {
            texture_atlas.insertTexture(glyph_texture),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x/64.0f
        };
    }

    texture_atlas.updateAtlas();
    FT_Done_FreeType(ft);
}

std::vector<unsigned char> TextSystem::bitmapToRGBA(unsigned char* data, int width, int height, int pitch) {
    std::vector<unsigned char> pixels;
    pixels.reserve(width*height*4);

    for (int y{0}; y < height; y++) {
        for (int x{0}; x < width; x++) {
            if (data[y*pitch+x/8] & 0b10000000>>(x%8)) {
                for (int it{0}; it < 4; it++) {
                    pixels.push_back(0xff);
                }
            } else {
                for (int it{0}; it < 4; it++) {
                    pixels.push_back(0x00);
                }
            }
        }
    }

    return pixels;
}

void TextSystem::emplaceTextures(entt::registry& registry, entt::entity entity) {
    assert((registry.all_of<Spacial, Text>(entity) && "Entity does not have Spacial"));
    
    auto& spacial{registry.get<Spacial>(entity)};
    auto& text{registry.get<Text>(entity)};

    float total_x_offset{0};

    for (auto c : text.text) {
        auto glyph_entity{registry.create()};

        // If the parent element is a GuiElement, make the glyph entities also GuiElements
        if (registry.all_of<GuiElement>(entity)) {
            registry.emplace<GuiElement>(glyph_entity);
        }

        if (registry.all_of<Component::Outline>(entity)) {
            registry.emplace<Component::Outline>(glyph_entity);
        }

        registry.emplace_or_replace<Component::Outline>(glyph_entity);

        // TODO: Remove this
        // registry.emplace<ComponentGridIgnore>(glyph_entity);

        FontCharacter& curr_char{this->fonts["Cozette"].characters[c]};

        glm::vec3 offset{total_x_offset+curr_char.bearing.x, -curr_char.bearing.y,0};

        registry.emplace<Spacial>(glyph_entity, spacial.pos+offset);
        registry.emplace<Texture>(glyph_entity,
            "Cozette",
            curr_char.frame_data
        );
        registry.emplace<Renderable>(glyph_entity);
        this->registry.emplace<Name>(glyph_entity, std::string(1, c));

        total_x_offset += curr_char.advance;
    }
}