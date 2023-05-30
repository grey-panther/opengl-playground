#version 330 core

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoords;

out vec4 vColor;
out vec2 vTexCoords;


void main() {
	vColor = aColor;
	vTexCoords = aTexCoords;
	gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}
