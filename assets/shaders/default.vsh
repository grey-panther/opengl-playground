#version 330 core

uniform float uOffsetX;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoords;

out vec4 vColor;
out vec2 vTexCoords;


void main() {
	vColor = aColor;
	vTexCoords = aTexCoords;
	gl_Position = vec4(aPos, 1.0);
	gl_Position.x += uOffsetX;
}
