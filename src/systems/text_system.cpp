#include "text_system.hpp"

#include <iostream>

TextSystem::TextSystem(entt::registry& registry) : System(registry) {
    
    TextureAtlas& texture_atlas = this->registry.ctx().at<TextureAtlas&>();

    // Use the following for copying the old atlas texture into the resized, taller texture:
    // glCopyImageSubData

    // Use the following for copying the bitmap pixel data into the newly resized texture
    // glTextureSubImage2D

    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        #ifndef NDEBUG
            std::cerr << "ERROR: Could not initialize FreeType" << std::endl;
        #endif
    }

    FT_Face face;

    std::string font_path = "./assets/fonts/cozette/cozette_bitmap.ttf";
    if (FT_New_Face(ft, font_path.c_str(), 0, &face)) {
        #ifndef NDEBUG
            std::cerr << "ERROR: FreeType failed to load font: " << font_path << std::endl;  
        #endif
    }

    std::cout << "width: " << face->available_sizes[0].width << " height: " << face->available_sizes[0].height << "\n";

    // int num_glyphs_to_load{256};
    // int glyph_max_width{face->available_sizes[0].width};
    // int glyph_max_height{face->available_sizes[0].height};

    // int glyphs_per_row{texture_atlas.width/glyph_max_width};
    // int num_rows_of_glyphs{(num_glyphs_to_load-1)/glyphs_per_row + 1};
    // int new_atlas_height{texture_atlas.height+num_rows_of_glyphs*glyph_max_height};

    // std::vector<GLuint> new_texture_atlas_pixels(texture_atlas.width*new_atlas_height*4);

    // glBindTexture(GL_TEXTURE_2D, texture_atlas.gl_texture_id);
    // glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_INT, new_texture_atlas_pixels.data());

    // 
    // glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
    
    // for (unsigned char c = 0; c < 128; c++)
    // {
    //     // load character glyph 
    //     if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    //     {
    //         std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
    //         continue;
    //     }
    //     // generate texture
    //     unsigned int texture;
    //     glGenTextures(1, &texture);
    //     glBindTexture(GL_TEXTURE_2D, texture);
    //     glTexImage2D(
    //         GL_TEXTURE_2D,
    //         0,
    //         GL_RED,
    //         face->glyph->bitmap.width,
    //         face->glyph->bitmap.rows,
    //         0,
    //         GL_RED,
    //         GL_UNSIGNED_BYTE,
    //         face->glyph->bitmap.buffer
    //     );
    //     // set texture options
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //     // now store character for later use
    //     Character character = {
    //         texture, 
    //         glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
    //         glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
    //         face->glyph->advance.x
    //     };
    //     Characters.insert(std::pair<char, Character>(c, character));
    // }
}

void TextSystem::update() {}