#version 330 core
in vec2 TexCoords;
flat in int TileID;
out vec4 color;

uniform sampler2D image;
uniform vec2 texData; // startX, endX
uniform vec3 spriteColor;

// float rand(vec2 co) {

//     return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
// }

void main() {

    float tilePart = 0.03125; // 1/32 or number of tiles per row
    float bleedOffset = 0.000001; // There may be a better solution that exists to avoid texture bleeding

    // Assume 512x512 px map of 32x32 tiles
    int xID = TileID % 32;
    int yID = TileID / 32;

    float framePercentage = (texData.y-texData.x);

    float xPercentage = framePercentage*tilePart - 2*bleedOffset;
    float xOffset = texData.x + (tilePart*xID)*framePercentage + bleedOffset;

    float yPercentage = tilePart - 2*bleedOffset;
    float yOffset = tilePart*yID + bleedOffset;

    // float xNoise = rand(vec2(TexCoords.x, TexCoords.y))/3000;
    // float yNoise = rand(vec2(TexCoords.y, TexCoords.x))/3000;

    color = vec4(spriteColor, 1.0) * texture(image, vec2(TexCoords.x*xPercentage + xOffset, TexCoords.y*yPercentage + yOffset));
    // color = vec4(spriteColor, 1.0) * texture(image, vec2(TexCoords.x*tilePart + tilePart*xID, TexCoords.y*tilePart + tilePart*yID));
    // color = vec4(spriteColor, 1.0) * texture(image, vec2(TexCoords.x * (texData.y-texData.x) + texData.x, TexCoords.y));
}  