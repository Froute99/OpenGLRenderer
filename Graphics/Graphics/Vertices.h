/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: VertexObject.h
 *	
 *	VertexObject class that manage vertices
 *	
 *	Fall 2019
 *	Oct.31 2019
 */

#pragma once
#include "VerticesDescription.h"

class Mesh;
class Mesh3D;

class [[nodiscard]] VertexObject
{
public:
	VertexObject() = default;
	VertexObject(const Mesh3D& mesh, const VerticesDescription& vertex_layout) noexcept;

	void InitializeWithMeshAndLayout(const Mesh3D& mesh, const VerticesDescription& vertex_layout) noexcept;
	void UpdateVeticesFromMesh(const Mesh& mesh);

	static void SelectVAO(const VertexObject& vertices) noexcept;
	static void SelectNothing();

	unsigned int GetPattern() const noexcept;
	int GetVerticesCount() const noexcept;
	
	void WriteMeshDataToVertexBuffer(const Mesh& mesh) const noexcept;
	void WriteMeshDataToVertexBuffer3D(const Mesh3D& mesh) const noexcept;
	void DeleteVerticesOnGPU() const;

	unsigned int VBO = 0;
	unsigned int VAO = 0;
private:

	unsigned int pattern = 0;
	int bufferVertexCapacity = 0;
	int verticesCount = 0;
	VerticesDescription layout{};
};

