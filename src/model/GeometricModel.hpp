#pragma once

#include "VertexFormat.hpp"

#include <vector>


class GeometricModel
{
public:
	using IndexType = unsigned int;

	GeometricModel(std::vector<VertexFormat> vertices, std::vector<IndexType> indices)
			: _vertices(std::move(vertices))
			, _indices(std::move(indices))
	{
	}

	[[nodiscard]]
	const std::vector<VertexFormat>& GetVertices() const { return _vertices; };

	[[nodiscard]]
	const std::vector<IndexType>& GetIndices() const { return _indices; };

private:
	std::vector<VertexFormat> _vertices;
	std::vector<IndexType> _indices;
};
