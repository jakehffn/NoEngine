#version 330 core

in vec2 texCoords; // Value from 0-1
in vec4 textureData; // x, y, width, height

out vec4 color;

uniform sampler2D image;
uniform vec2 atlasDimensions;


void main() {

    float bleedOffset = 0.000001; // There may be a better solution that exists to avoid texture bleeding

    float xPercentage = textureData.z/atlasDimensions.x - 2*bleedOffset;
    float xOffset = textureData.x/atlasDimensions.x + bleedOffset;

    float yPercentage = textureData.w/atlasDimensions.y - 2*bleedOffset;
    float yOffset = textureData.y/atlasDimensions.y + bleedOffset;

    color = texture(image, vec2(texCoords.x*xPercentage + xOffset, texCoords.y*yPercentage + yOffset));
    
    // color = texture(image, vec2(texCoords.x, texCoords.y));
    // color = texture(image, vec2(texCoords.x/8, texCoords.y*yPercentage + yOffset));
}  