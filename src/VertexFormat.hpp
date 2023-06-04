#pragma once


struct VertexFormat {
	struct Pos {
		float x = 0.f;
		float y = 0.f;
		float z = 0.f;
	};
	struct Color {
		float r = 0.f;
		float g = 0.f;
		float b = 0.f;
		float a = 0.f;
	};
	struct TextureCoords {
		float u = 0.f;
		float v = 0.f;
	};
	Pos pos;
	Color color;
	TextureCoords texCoords;
};
