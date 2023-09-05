#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>
layout (location = 1) in vec4 instanceTextureData; // 
layout (location = 3) in mat4 instanceModel;

out vec2 texCoords;
out vec4 textureData;

uniform mat4 V;
uniform mat4 P;

float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

void main() {

	textureData = instanceTextureData;
	texCoords = vertex.zw;
	
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  P*V*instanceModel * vec4(vertex.xy, 1.0, 1.0);


	// gl_Position =  P*V*instanceModel * vec4(vertex.xy*8, 1.0, 1.0);
}