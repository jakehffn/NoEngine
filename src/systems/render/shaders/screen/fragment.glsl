#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D screen_texture;
uniform vec2 screen_resolution;
uniform float time;

vec4 sampleTexture(float x, float y) {
    vec2 sample_pixel_center = vec2(ivec2(x, y)) + vec2(0.5, 0.5);
    return texture(screen_texture, sample_pixel_center/screen_resolution);
}

void main() {   
    vec2 pixel = vec2(TexCoords.x, 1-TexCoords.y) * screen_resolution;
    color = sampleTexture(pixel.x, pixel.y);
} 