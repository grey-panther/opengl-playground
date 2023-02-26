#version 330 core

uniform sampler2D sampler0;
uniform sampler2D sampler1;
uniform float uProgress;

in vec4 vColor;
in vec2 vTexCoords;

out vec4 fragColor;


void main() {
	vec4 color0 = texture(sampler0, vTexCoords);
	vec2 texCoords1 = vec2(1.f - vTexCoords.x * 2, vTexCoords.y * 2);
	vec4 color1 = texture(sampler1, texCoords1);
	fragColor = mix(color0, color1, uProgress);
}
