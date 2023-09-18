#version 330 core

#define M_PI 3.1415926535897932384626433832795

in vec2 texture_coords; // Value from 0-1
in vec4 texture_data; // x, y, width, height

out vec4 color;

uniform sampler2D image;
uniform vec2 atlas_dimensions;

uniform float time;

vec4 sampleTexture(float x, float y) {
    vec2 sample_pixel_center = texture_data.xy + vec2(ivec2(x, y)) + vec2(0.5, 0.5);
    return texture(image, sample_pixel_center/atlas_dimensions);
}

vec4 radialColor(float x, float y) {
    float time_reduce = 100;
    float width = M_PI/4;
    vec4 main_color = vec4(1, 1, 1, 1);
    vec4 accent_color = vec4(0.9, 0.9, 0.9, 1);

    float swirl_amount = 0;

    vec2 centered_pixel = vec2(ivec2(x, y)) - texture_data.zw/2;
    float swirl_offset = distance(vec2(0, 0), centered_pixel/texture_data.zw) * swirl_amount;
    float texture_angle = mod(atan(centered_pixel.y, centered_pixel.x) + swirl_offset, M_PI);
    float current_colored_angle = mod(time/time_reduce, 2*M_PI) - M_PI;
    float angle_diff = min(abs(current_colored_angle - texture_angle), 2*M_PI - abs(current_colored_angle - texture_angle));

    if (angle_diff < width || angle_diff > M_PI - width) {
        return accent_color;
    } else {
        return main_color;
    }
}

void main() {
    // The number of pixels of border space given by the vertex shader
    float border_space = 2;

    float pixel_x = texture_coords.x * (texture_data.z + border_space) - (border_space/2);
    float pixel_y = texture_coords.y * (texture_data.w + border_space) - (border_space/2);
    
    vec4 outline_color = radialColor(pixel_x, pixel_y);
    vec4 clear_color = vec4(0, 0, 0, 0);

    bool is_near_color = false;
    
    if (pixel_x > 0 && pixel_y > 0 && pixel_x < texture_data.z && pixel_y < texture_data.w) {
        vec4 current_color = sampleTexture(pixel_x, pixel_y);

        if (current_color.a > 0) {
            color = clear_color;
        } else {
            // Adjacent
            if (pixel_x < texture_data.z - 1) {
                is_near_color = is_near_color || (sampleTexture(pixel_x+1, pixel_y).a > 0);
            } 
            if (pixel_y < texture_data.w - 1) {
                is_near_color = is_near_color || (sampleTexture(pixel_x, pixel_y+1).a > 0);
            } 
            if (pixel_x > 1) {
                is_near_color = is_near_color || (sampleTexture(pixel_x-1, pixel_y).a > 0);
            }
            if (pixel_y > 1) {
                is_near_color = is_near_color || (sampleTexture(pixel_x, pixel_y-1).a > 0);
            }
            // Diagonal
            if (pixel_x < texture_data.z - 1 && pixel_y < texture_data.w - 1) {
                is_near_color = is_near_color || (sampleTexture(pixel_x+1, pixel_y+1).a > 0);
            } 
            if (pixel_y < texture_data.w - 1 && pixel_x > 1) {
                is_near_color = is_near_color || (sampleTexture(pixel_x-1, pixel_y+1).a > 0);
            } 
            if (pixel_x > 1 && pixel_y > 1) {
                is_near_color = is_near_color || (sampleTexture(pixel_x-1, pixel_y-1).a > 0);
            }
            if (pixel_y > 1 && pixel_x < texture_data.z - 1) {
                is_near_color = is_near_color || (sampleTexture(pixel_x+1, pixel_y-1).a > 0);
            }
            if (is_near_color) {
                color = outline_color;
            } else {
                color = clear_color;
            }
        }
    } else {
        // Adjacent
        if (pixel_x < 0 && pixel_y > 0 && pixel_y < texture_data.w) {
            is_near_color = is_near_color || (sampleTexture(pixel_x+1, pixel_y).a > 0);
        } 
        if (pixel_y < 0 && pixel_x > 0 && pixel_x < texture_data.z) {
            is_near_color = is_near_color || (sampleTexture(pixel_x, pixel_y+1).a > 0);
        } 
        if (pixel_x > texture_data.z && pixel_y > 0 && pixel_y < texture_data.w) {
            is_near_color = is_near_color || (sampleTexture(pixel_x-1, pixel_y).a > 0);
        }
        if (pixel_y > texture_data.w && pixel_x > 0 && pixel_x < texture_data.z) {
            is_near_color = is_near_color || (sampleTexture(pixel_x, pixel_y-1).a > 0);
        }
        // Diagonal
        if (pixel_x < texture_data.z - 1 && pixel_y < texture_data.w - 1) {
            is_near_color = is_near_color || (sampleTexture(pixel_x+1, pixel_y+1).a > 0);
        } 
        if (pixel_y < texture_data.w - 1 && pixel_x > 1) {
            is_near_color = is_near_color || (sampleTexture(pixel_x-1, pixel_y+1).a > 0);
        } 
        if (pixel_x > 1 && pixel_y > 1) {
            is_near_color = is_near_color || (sampleTexture(pixel_x-1, pixel_y-1).a > 0);
        }
        if (pixel_y > 1 && pixel_x < texture_data.z - 1) {
            is_near_color = is_near_color || (sampleTexture(pixel_x+1, pixel_y-1).a > 0);
        }
        if (is_near_color) {
            color = outline_color;
        } else {
            color = clear_color;
        }
    }
}  