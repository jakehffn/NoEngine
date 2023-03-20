#include "texture_atlas.hpp"

TextureAtlas::TextureAtlas() {
    glGenTextures(1, &(this->gl_texture_id));
}

AtlasData* TextureAtlas::insertTexture(const TextureSource& source) {

    // Format is always converted to RGBA

    int format_size{0};
    const int dest_format_size{4};
    int num_texels{source.size.x*source.size.y*dest_format_size};
    std::vector<unsigned char> new_source_data(num_texels);

    switch(source.data_format) {

        case GL_RGBA:

            format_size = 4;

            for (int y{0}; y < source.size.y; y++) {
                for (int x{0}; x < source.size.x*format_size; x++) {

                    unsigned char val{source.data[(y+source.source_offset.y)*source.data_size.x*format_size + x + source.source_offset.x*format_size]};
                    new_source_data[y*source.size.x*format_size + x] = val;     
                }
            }

            break;

        case GL_RGB:

            format_size = 3;

            for (int y{0}; y < source.size.y; y++) {
                for (int x{0}; x < source.size.x*format_size; x++) {

                    unsigned char val{source.data[(y+source.source_offset.y)*source.data_size.x*format_size + x + source.source_offset.x*format_size]};

                    // Emplace four values for every value
                    for (int it{0}; it < 3; it++) {
                        new_source_data[y*source.size.x*dest_format_size + x + it] = val;
                    }

                    new_source_data[y*source.size.x*dest_format_size + x + 3] = 0xFF;
                }
            }

            break;


        case GL_RED:

            for (int y{0}; y < source.size.y; y++) {
                for (int x{0}; x < source.size.x; x++) {

                    unsigned char val{source.data[(y+source.source_offset.y)*source.data_size.x + x + source.source_offset.x]};

                    // Emplace four values for every value
                    for (int it{0}; it < 3; it++) {
                        new_source_data[y*source.size.x*dest_format_size + x*dest_format_size + it] = val;
                    }

                    new_source_data[y*source.size.x*dest_format_size + x*dest_format_size + 3] = 0xFF;
                }
            }

            break;

        default:

            #ifndef NDEBUG
                std::cerr << "ERROR: Unhandled texture data format!\n";
            #endif

            return 0;
    };

    auto& new_atlas_data = this->atlas_data.emplace_back(glm::vec2(), source.size, source.offset);
    this->sources_data.emplace_back(this->sources.size(), &new_atlas_data);
    sources.emplace_back(new_source_data);

    return &new_atlas_data;
}

void TextureAtlas::updateAtlas() {
    
    this->updateAtlasDataPacking();
    this->updateAtlasTexture();
}

void TextureAtlas::updateAtlasDataPacking() {
    // Get the packing of the AtlasData rectangles
    constexpr bool allow_flip = false;
    const auto runtime_flipping_mode = rectpack2D::flipping_option::DISABLED;
    using spaces_type = rectpack2D::empty_spaces<allow_flip, rectpack2D::default_empty_spaces>;
    using rect_type = rectpack2D::output_rect_t<spaces_type>;

    auto report_successful = [](rect_type&) {
		return rectpack2D::callback_result::CONTINUE_PACKING;
	};

	auto report_unsuccessful = [](rect_type&) {

        #ifndef NDEBUG
            std::cerr << "ERROR: Unsuccessful insertion rectpack2D" << "\n";
        #endif

		return rectpack2D::callback_result::ABORT_PACKING;
	};

    const auto max_side = 2048;
    const auto discard_step = 1;

    std::vector<rect_type> rectangles;
    rectangles.reserve(this->atlas_data.size());

    for (auto texture : this->atlas_data) {
        rectangles.emplace_back(0, 0, texture.size.x, texture.size.y);
    }

    const auto result_size = rectpack2D::find_best_packing<spaces_type>(rectangles, rectpack2D::make_finder_input(
				max_side,
				discard_step,
				report_successful,
				report_unsuccessful,
				runtime_flipping_mode
	));
    
    this->width = result_size.w;
    this->height = result_size.h;

    #ifndef NDEBUG
        std::cout << "Resulting texture size: " << result_size.w << " " << result_size.h << "\n";
    #endif

    // Assign the generated positions to the atlas datas
    auto rect_it{rectangles.begin()};
    auto atlas_data_it{this->atlas_data.begin()};

    while (atlas_data_it != atlas_data.end()) {

        atlas_data_it->position = glm::ivec2(rect_it->x, rect_it->y);
        rect_it++;
        atlas_data_it++;
    }
}

void TextureAtlas::updateAtlasTexture() {

    std::vector<unsigned char> empty_texture_source(this->width*this->height*4);
    
    glBindTexture(GL_TEXTURE_2D, this->gl_texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, empty_texture_source.data());
    

    for (auto source_data_it{this->sources_data.begin()}; source_data_it != this->sources_data.end(); source_data_it++) {

        auto& source = sources[source_data_it->source_index];
        auto& atlas_loc = *(source_data_it->atlas_data);
        glTexSubImage2D(
            GL_TEXTURE_2D, 
            0, 
            atlas_loc.position.x, 
            atlas_loc.position.y, 
            atlas_loc.size.x, 
            atlas_loc.size.y,
            GL_RGBA, 
            GL_UNSIGNED_BYTE,
            source.data()
        );
    };

    glGenerateMipmap(GL_TEXTURE_2D);
}