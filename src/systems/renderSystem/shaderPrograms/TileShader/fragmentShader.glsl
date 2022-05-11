#version 330 core
in vec2 TexCoords;
flat in int TileID;
out vec4 color;

uniform sampler2D image;
uniform vec2 texData; // startX, endX
uniform vec3 spriteColor;

void main()
{    
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

    color = vec4(spriteColor, 1.0) * texture(image, vec2(TexCoords.x*xPercentage + xOffset, TexCoords.y*yPercentage + yOffset));
    // color = vec4(spriteColor, 1.0) * texture(image, vec2(TexCoords.x*tilePart + tilePart*xID, TexCoords.y*tilePart + tilePart*yID));
    // color = vec4(spriteColor, 1.0) * texture(image, vec2(TexCoords.x * (texData.y-texData.x) + texData.x, TexCoords.y));
}  