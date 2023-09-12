#version 330 core

in vec2 texture_coords; // Value from 0-1
in vec4 texture_data; // x, y, width, height

out vec4 color;

uniform sampler2D image;
uniform vec2 atlas_dimensions;

void main() {
    // The number of pixels of border space given by the vertex shader
    float border_space = 2;

    float pixel_x = texture_coords.x * texture_data.z;
    float pixel_y = texture_coords.y * texture_data.w;
    
    vec4 outline_color = vec4(0.2, 0.8, 0.6, 0.8);
    vec4 fill_color = vec4(0.2, 0.8, 0.6, 0.2);
    float border_width = 0.8;
    
    if (pixel_x > border_width && pixel_y > border_width && pixel_x < texture_data.z - border_width && pixel_y < texture_data.w - border_width) {
        color = fill_color;
    } else {
        color = outline_color;
    }
}  