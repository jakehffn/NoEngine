#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;
uniform vec2 texData; // startX, endX
uniform vec3 spriteColor;

float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

void main() {   

    // vec2 offset = fract(vec2(random(TexCoords), random(TexCoords.yx)))/700;
    vec2 offset = vec2(sin(TexCoords.x*10), sin(TexCoords.y*10))/60;

    // vec3 col = texture(screenTexture, vec2(TexCoords.x + offset.x, 1-TexCoords.y + offset.y)).rgb;
    vec3 col = texture(screenTexture, vec2(TexCoords.x, 1-TexCoords.y)).rgb;
    color = vec4(col, 1.0);
    // color = vec4(1.0, 1.0, 1.0, 1.0);
}  