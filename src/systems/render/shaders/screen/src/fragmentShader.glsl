#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;
uniform float time;

float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

void main() {   

    // vec2 offset = fract(vec2(random(TexCoords), random(TexCoords.yx)))/7;
    // vec2 offset = vec2(sin(TexCoords.x*100), sin(TexCoords.y*10))/600;

    // vec3 col = texture(screenTexture, vec2(TexCoords.x + offset.x, 1-TexCoords.y + offset.y)).rgb;

    // Strobe lights !!
    // vec3 col = texture(screenTexture, vec2(TexCoords.x, 1-TexCoords.y)).rgb;
    // col = vec3(col.r, col.bg*(sin(time/50)/4 + 0.8));

    vec3 col = texture(screenTexture, vec2(TexCoords.x, 1-TexCoords.y)).rgb;
    // float mod_val = mod(time / 1000000.0, 0.05);
    // vec3 col = texture(screenTexture, vec2(TexCoords.x - mod(TexCoords.x - 0.5, mod_val), (1-TexCoords.y) - mod(1-TexCoords.y + 0.5, mod_val * 1.2))).rgb;
    color = vec4(col, 1.0);
}  