#version 330 core

in vec2 texture_coords; // Value from 0-1
in vec4 texture_data; // x, y, width, height

out vec4 color;

uniform sampler2D atlas_texture;
uniform vec2 atlas_dimensions;

void main() {
    vec2 sample_pixel_center = texture_data.xy + vec2(ivec2(texture_coords*texture_data.zw)) + vec2(0.5, 0.5);
    color = texture(atlas_texture, sample_pixel_center/atlas_dimensions);
}  