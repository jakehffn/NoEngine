#version 330 core
in vec2 TexCoords;
flat in int TileID;
out vec4 color;

uniform sampler2D image;
uniform vec2 texData; // startX, endX
uniform vec3 spriteColor;

void main()
{    
    float tilePart = 0.03125; // 1/32

    // Assume 512x512 px map of 32x32 tiles
    int xID = TileID % 32;
    int yID = TileID / 32;

    color = vec4(spriteColor, 1.0) * texture(image, vec2(TexCoords.x*tilePart + tilePart*xID, TexCoords.y*tilePart + tilePart*yID));
}  