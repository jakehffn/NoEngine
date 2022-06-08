#version 330 core

layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;

void main() {

	TexCoords = vertex.zw;
	// Convert from 0,0 being top left and 1,1 being bottom right to -1,1 being top left and 1,-1 being bottom right
	gl_Position = vec4(2*vertex.x - 1, 1 - 2*vertex.y, 0.0, 1.0);
	
}