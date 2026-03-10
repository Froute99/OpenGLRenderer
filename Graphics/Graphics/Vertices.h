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

// modify name to VertexUplodaer, uploading mesh information to video memory
class [[nodiscard]] VertexObject
{
public:
	VertexObject() = default;
	~VertexObject();
	VertexObject(const Mesh3D* mesh, const VerticesDescription& vertex_layout) noexcept;

	void InitializeWithMeshAndLayout(const Mesh3D& mesh, const VerticesDescription& vertex_layout) noexcept;
	void UpdateVeticesFromMesh(const Mesh& mesh);

	void SetLayout(const VerticesDescription& newLayout) noexcept { layout = newLayout; }

	static void SelectVAO(const VertexObject& vertices) noexcept;
	static void SelectNothing();

	unsigned int GetPattern() const noexcept { return pattern; }
	int			 GetVerticesCount() const noexcept { return verticesCount; }
	int			 GetIndicesCount() const noexcept { return indicesCount; }

	void WriteMeshDataToVertexBuffer3D(const Mesh3D& mesh) const noexcept;
	void DeleteVerticesOnGPU() const;

	unsigned int VBO = 0;
	unsigned int VAO = 0;
	unsigned int EBO = 0;

private:
	unsigned int		pattern = 0;
	int					bufferVertexCapacity = 0;
	int					verticesCount = 0;
	int					indicesCount = 0;
	VerticesDescription layout{};
};
