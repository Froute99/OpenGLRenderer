#pragma once
#include "Mesh.h"
#include <Graphics/Color4f.h>
#include <Math/vec2.hpp>
#include <Math/vec3.hpp>
#include <vector>
#include <string>

class Mesh3D
{
public:
	void AddPoint(vec3<float> point) noexcept { points.push_back(point); }
	void AddNormal(vec3<float> normal) noexcept { normals.push_back(normal); }
	void AddColor(Color4f color) noexcept { colors.push_back(color); }
	void AddTexCoord(vec2<float> texCoord) noexcept { textureCoordinates.push_back(texCoord); }
	void AddIndex(unsigned int index) noexcept { indices.push_back(index); }
	void AddIndices(vec3<unsigned int> newIndices)
	{
		indices.push_back(newIndices.x);
		indices.push_back(newIndices.y);
		indices.push_back(newIndices.z);
	}

	vec3<float> GetPoint(int index) const noexcept { return points.at(index); }
	vec3<float> GetNormal(int index) const noexcept { return normals.at(index); }
	vec2<float> GetTextureCoordinate(int index) const noexcept { return textureCoordinates.at(index); }
	Color4f		GetColor(int index) const noexcept { return colors.at(index); }

	void		 SetShapePattern(ShapePattern newPattern) noexcept { pattern = newPattern; }
	ShapePattern GetShapePattern() const noexcept { return pattern; }

	int					GetPointsCount() const noexcept { return static_cast<int>(points.size()); }
	int					GetIndicesCount() const noexcept { return static_cast<int>(indices.size()); }
	const unsigned int* GetIndicesData() const noexcept { return indices.data(); }

private:
	std::vector<vec3<float>>  points;
	std::vector<vec3<float>>  normals;
	std::vector<vec2<float>>  textureCoordinates;
	std::vector<Color4f>	  colors;
	std::vector<unsigned int> indices;
	ShapePattern			  pattern = ShapePattern::Triangles;
};

namespace MESH
{
	Mesh3D	LoadFromFBX(const std::string& path);
	Mesh3D* BuildCube(float size, vec3<float> color);
	Mesh3D* BuildCube(float size, Color4f color = { 0.5f });
	Mesh3D* BuildSphere();
} // namespace MESH
