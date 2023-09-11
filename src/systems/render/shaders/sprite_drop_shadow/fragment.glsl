#version 330 core

in vec2 texture_coords; // Value from 0-1
in vec4 texture_data; // x, y, width, height

out vec4 color;

uniform sampler2D image;
uniform vec2 atlas_dimensions;

vec4 sampleTexture(float x, float y) {
    float bleed_offset = 0.000001; // There may be a better solution that exists to avoid texture bleeding

    float x_percentage = texture_data.z/atlas_dimensions.x - 2*bleed_offset;
    float x_offset = texture_data.x/atlas_dimensions.x + bleed_offset;

    float y_percentage = texture_data.w/atlas_dimensions.y - 2*bleed_offset;
    float y_offset = texture_data.y/atlas_dimensions.y + bleed_offset;

    vec2 texture_pixel = vec2((x/texture_data.z)*x_percentage + x_offset, (y/texture_data.w)*y_percentage + y_offset);
    return texture(image, texture_pixel);
}

void main() {
    // The number of pixels of border space given by the vertex shader
    float border_space = 2;

    float pixel_x = texture_coords.x * (texture_data.z + border_space) - (border_space/2);
    float pixel_y = texture_coords.y * (texture_data.w + border_space) - (border_space/2);
    
    vec4 outline_color = vec4(0, 0, 0, 1);
    bool is_near_color = false;
    
    if (pixel_x > 0 && pixel_y > 0 && pixel_x < texture_data.z && pixel_y < texture_data.w) {
        vec4 current_color = sampleTexture(pixel_x, pixel_y);

        if (current_color.a > 0) {
            // color = current_color;
        } else {
            // Adjacent
            if (pixel_x > 1) {
                is_near_color = is_near_color || (sampleTexture(pixel_x-1, pixel_y).a > 0);
            }
            if (pixel_y > 1) {
                is_near_color = is_near_color || (sampleTexture(pixel_x, pixel_y-1).a > 0);
            }
            // Diagonal
            if (pixel_x > 1 && pixel_y > 1) {
                is_near_color = is_near_color || (sampleTexture(pixel_x-1, pixel_y-1).a > 0);
            }
            if (is_near_color) {
                color = outline_color;
            }
        }
    } else {
        // Adjacent
        if (pixel_x > texture_data.z && pixel_y > 0 && pixel_y < texture_data.w) {
            is_near_color = is_near_color || (sampleTexture(pixel_x-1, pixel_y).a > 0);
        }
        if (pixel_y > texture_data.w && pixel_x > 0 && pixel_x < texture_data.z) {
            is_near_color = is_near_color || (sampleTexture(pixel_x, pixel_y-1).a > 0);
        }
        // Diagonal
        if (pixel_x > 1 && pixel_y > 1) {
            is_near_color = is_near_color || (sampleTexture(pixel_x-1, pixel_y-1).a > 0);
        }
        if (is_near_color) {
            color = outline_color;
        }
    }
}  