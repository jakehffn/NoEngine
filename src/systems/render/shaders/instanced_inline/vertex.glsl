#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>
layout (location = 1) in vec4 instance_texture_data; // 
layout (location = 3) in mat4 instance_model;

out vec2 texture_coords;
out vec4 texture_data;

uniform mat4 V;
uniform mat4 P;

void main() {
	texture_data = instance_texture_data;
	texture_coords = vertex.zw;
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  P*V*instance_model * vec4(vertex.xy, 1.0, 1.0);
}