#include "GeometricModelFactory.hpp"


[[nodiscard]]
GeometricModel GeometricModelFactory::createRectangleModel()
{
	auto vertices = std::vector<VertexFormat> {
			VertexFormat{{-1.f, -1.f, 0.f}, {1.f, 0.f, 0.f, 1.f}, {0.f, 0.f}},		// Left bottom
			VertexFormat{{1.f, -1.f, 0.f}, {0.f, 1.f, 0.f, 1.f}, {1.f, 0.f}},		// Right bottom
			VertexFormat{{-1.f, 1.f, 0.f}, {0.f, 0.f, 1.f, 1.f}, {0.f, 1.f}},		// Left top
			VertexFormat{{1.f, 1.f, 0.f}, {0.f, 1.f, 1.f, 1.f}, {1.f, 1.f}},		// Right top
	};

	auto indices = std::vector<GeometricModel::IndexType> {
			0, 1, 2,
			1, 2, 3,
	};

	return GeometricModel(std::move(vertices), std::move(indices));
}


[[nodiscard]]
GeometricModel GeometricModelFactory::createCubeModel()
{
	VertexFormat::Color color = {1.f, 1.f, 1.f, 1.f};

	// Cube
	//   7---6     11--14      *---*     23---22
	//  /|  /|    /|  /|      /|  /|     /|  /|
	// 3 4-2-5  10 8-15-13   * 18*-19  20-*-21*
	// |/  |/    |/  |/      |/  |/     |/  |/
	// 0---1     9---12     17---16     *---*

	auto vertices = std::vector<VertexFormat> {
			// front
			VertexFormat{{-1.f, -1.f, 1.f}, color, {0.f, 0.f}},
			VertexFormat{{1.f, -1.f, 1.f}, color, {1.f, 0.f}},
			VertexFormat{{1.f, 1.f, 1.f}, color, {1.f, 1.f}},
			VertexFormat{{-1.f, 1.f, 1.f}, color, {0.f, 1.f}},
			// back
			VertexFormat{{-1.f, -1.f, -1.f}, color, {1.f, 0.f}},
			VertexFormat{{1.f, -1.f, -1.f}, color, {0.f, 0.f}},
			VertexFormat{{1.f, 1.f, -1.f}, color, {0.f, 1.f}},
			VertexFormat{{-1.f, 1.f, -1.f}, color, {1.f, 1.f}},
			// left
			VertexFormat{{-1.f, -1.f, -1.f}, color, {0.f, 0.f}},
			VertexFormat{{-1.f, -1.f, 1.f}, color, {1.f, 0.f}},
			VertexFormat{{-1.f, 1.f, 1.f}, color, {1.f, 1.f}},
			VertexFormat{{-1.f, 1.f, -1.f}, color, {0.f, 1.f}},
			// right
			VertexFormat{{1.f, -1.f, 1.f}, color, {0.f, 0.f}},
			VertexFormat{{1.f, -1.f, -1.f}, color, {1.f, 0.f}},
			VertexFormat{{1.f, 1.f, -1.f}, color, {1.f, 1.f}},
			VertexFormat{{1.f, 1.f, 1.f}, color, {0.f, 1.f}},
			// bottom
			VertexFormat{{1.f, -1.f, 1.f}, color, {0.f, 0.f}},
			VertexFormat{{-1.f, -1.f, 1.f}, color, {1.f, 0.f}},
			VertexFormat{{-1.f, -1.f, -1.f}, color, {1.f, 1.f}},
			VertexFormat{{1.f, -1.f, -1.f}, color, {0.f, 1.f}},
			// top
			VertexFormat{{-1.f, 1.f, 1.f}, color, {0.f, 0.f}},
			VertexFormat{{1.f, 1.f, 1.f}, color, {1.f, 0.f}},
			VertexFormat{{1.f, 1.f, -1.f}, color, {1.f, 1.f}},
			VertexFormat{{-1.f, 1.f, -1.f}, color, {0.f, 1.f}},
	};

	auto indices = std::vector<GeometricModel::IndexType> {
			0, 1, 2,	// front
			0, 2, 3,
			5, 4, 7,	// back
			5, 7, 6,
			8, 9, 10,	// left
			8, 10, 11,
			12, 13, 14,	// right
			12, 14, 15,
			16, 17, 18,	// bottom
			16, 18, 19,
			20, 21, 22,	// top
			20, 22, 23,
	};

	return GeometricModel(std::move(vertices), std::move(indices));
}
