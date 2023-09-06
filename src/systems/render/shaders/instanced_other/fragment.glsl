#version 330 core

in vec2 texture_coords; // Value from 0-1
in vec4 texture_data; // x, y, width, height

out vec4 color;

uniform sampler2D image;
uniform vec2 atlas_dimensions;


void main() {
    float bleed_offset = 0.000001; // There may be a better solution that exists to avoid texture bleeding

    float x_percentage = texture_data.z/atlas_dimensions.x - 2*bleed_offset;
    float x_offset = texture_data.x/atlas_dimensions.x + bleed_offset;

    float y_percentage = texture_data.w/atlas_dimensions.y - 2*bleed_offset;
    float y_offset = texture_data.y/atlas_dimensions.y + bleed_offset;

    color = texture(image, vec2(texture_coords.x*x_percentage + x_offset, texture_coords.y*y_percentage + y_offset));
}  