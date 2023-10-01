#version 330 core

#define M_PI 3.1415926535897932384626433832795

in vec2 texture_coords; // Value from 0-1
in vec4 texture_data; // x, y, width, height

out vec4 color;

uniform sampler2D atlas_texture;
uniform vec2 atlas_dimensions;

uniform float time;

void dialogBoxStyle1() {
    float border_space = 2;
    vec4 background = vec4(0.05, 0.05, 0.05, 1);
    vec4 accent = vec4(0.95, 0.95, 0.95, 1);
    vec4 none = vec4(0);

    ivec2 pixels = ivec2(texture_coords.xy * texture_data.zw);

    if (pixels.x > 0 && pixels.y > 0 && pixels.x < texture_data.z - 1 && pixels.y < texture_data.w - 1) {
        color = background;
    } else {
        color = accent;
    }
}

void dialogBoxStyle2() {
    float border_space = 2;
    vec4 background = vec4(0.05, 0.05, 0.05, 1);
    vec4 accent = vec4(0.95, 0.95, 0.95, 1);
    vec4 none = vec4(0);

    ivec2 pixels = ivec2(texture_coords.xy * texture_data.zw);

    if (pixels.x > 4 && pixels.y > 4 && pixels.x < texture_data.z - 4 && pixels.y < texture_data.w - 4) {
        color = background;
    } else {
        if (pixels.x > 2 && pixels.y > 2 && pixels.x < texture_data.z - 1 && pixels.y < texture_data.w - 1) { 
            if (pixels.x == 3 || pixels.y == 3 || pixels.x == texture_data.z - 2 || pixels.y == texture_data.w - 2) {
                color = background;
            } else {
                color = accent;
            }
        } else {
            color = accent;
        }
    }
}

void main() {
    dialogBoxStyle2();
}  