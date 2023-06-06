#pragma once

#include "GeometricModel.hpp"


class GeometricModelFactory
{
public:
	[[nodiscard]]
	static GeometricModel createRectangleModel();

	[[nodiscard]]
	static GeometricModel createCubeModel();
};
