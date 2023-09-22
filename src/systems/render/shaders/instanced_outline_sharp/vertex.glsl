#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>
layout (location = 1) in vec4 instance_texture_data; // 
layout (location = 3) in mat4 instance_model;

out vec2 texture_coords;
out vec4 texture_data;

uniform mat4 V;
uniform mat4 P;

uniform vec2 screen_resolution;
uniform float camera_zoom;

void main() {
	float pixel_scale = 3;
	float xs = 1.0/1440 * pixel_scale * 2;
	float ys = 1.0/810 * pixel_scale * 2;

	vec4 scale_up = vec4(mix(-xs, xs, vertex.x), -mix(-ys, ys, vertex.y), 0, 0);

	texture_data = instance_texture_data;
	texture_coords = vertex.zw;

	// The verticies need to be scaled up so that the borders are drawable

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  P*V*instance_model * vec4(vertex.xy, 1.0, 1.0);
}