#pragma once

#include "GeometricModel.hpp"


class GeometricModelFactory
{
public:
	[[nodiscard]]
	static GeometricModel CreateRectangleModel();

	[[nodiscard]]
	static GeometricModel CreateCubeModel();
};
