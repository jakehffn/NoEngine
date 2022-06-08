#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec2 texData; // startX, endX
uniform vec3 spriteColor;

void main() {    
    color = vec4(spriteColor, 1.0) * texture(image, vec2(TexCoords.x * (texData.y-texData.x) + texData.x, TexCoords.y));
}  