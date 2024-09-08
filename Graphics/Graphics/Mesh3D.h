#pragma once
#include "Mesh.h"
#include "Color4f.hpp"
#include <vector>
#include <vec3.hpp>

class Mesh3D
{
public:
	void AddPoint(vec3<float> point) noexcept { points.push_back(point); }
	void AddNormal(vec3<float> normal) noexcept { normals.push_back(normal); }
	void AddColor(Color4f color) noexcept { colors.push_back(color); }
	void AddTexCoord(vec2<float> texCoord) noexcept { textureCoordinates.push_back(texCoord); }

	vec3<float> GetPoint(int index) const noexcept { return points.at(index); }
	vec3<float> GetNormal(int index) const noexcept { return normals.at(index); }
	Color4f		GetColor(int index) const noexcept { return colors.at(index); }

	void		 SetShapePattern(ShapePattern newPattern) noexcept { pattern = newPattern; }
	ShapePattern GetShapePattern() const noexcept { return pattern; }

	int GetPointsCount() const noexcept { return static_cast<int>(points.size()); }

private:
	std::vector<vec3<float>> points;
	std::vector<vec3<float>> normals;
	std::vector<vec2<float>> textureCoordinates;
	std::vector<Color4f>	 colors;
	ShapePattern			 pattern = ShapePattern::Triangles;
};

namespace MESH
{
	Mesh3D BuildCube(vec3<float> center, float size, vec3<float> color = vec3<float>(0.5f));
	Mesh3D BuildCube(vec3<float> center, float size, Color4f color = { 0.5f });
}
