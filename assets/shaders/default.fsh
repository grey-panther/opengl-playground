#version 330 core

uniform sampler2D sampler0;
uniform sampler2D sampler1;
uniform float uProgress;

in vec4 vColor;
in vec2 vTexCoords;

out vec4 fragColor;


void main() {
	vec4 color0 = texture(sampler0, vTexCoords);
	vec4 color1 = texture(sampler1, vTexCoords);
	fragColor = mix(color0, color1, uProgress);
}
