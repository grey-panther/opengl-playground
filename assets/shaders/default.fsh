#version 330 core

uniform sampler2D sampler;

in vec4 vColor;
in vec2 vTexCoords;

out vec4 fragColor;


void main() {
	fragColor = texture(sampler, vTexCoords) * vColor;
}
