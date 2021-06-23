#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec2 texData; // offset, fraction
uniform vec3 spriteColor;

void main()
{    
    color = vec4(spriteColor, 1.0) * texture(image, vec2((texData.x * texData.y) + (TexCoords.x * texData.y), TexCoords.y));
}  