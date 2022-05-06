#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>
layout (location = 1) in vec3 instance; // <vec2 position, float tileID>

out vec2 TexCoords;
flat out int TileID;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;

void main(){

	TexCoords = vertex.zw;

	TileID = int(instance.z);
	
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(vertex.xy + instance.xy, 1.0, 1.0);
	
}