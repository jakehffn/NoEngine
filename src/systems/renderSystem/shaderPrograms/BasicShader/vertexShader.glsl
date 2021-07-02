#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;

void main(){

	TexCoords = vertex.zw;
	
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(vertex.xy, 1.0, 1.0);
	
}