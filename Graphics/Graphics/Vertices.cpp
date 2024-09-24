/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: VertexObject.cpp
 *	
 *	VertexObject class that manage vertices
 *	
 *	Fall 2019
 *	Oct.31 2019
 */

#include "Vertices.h"
#include "Mesh.h"
#include "Mesh3D.h"
#include <glew.h>

VertexObject::VertexObject(const Mesh3D& /*mesh*/, const VerticesDescription& /*vertex_layout*/) noexcept
{
	//InitializeWithMeshAndLayout(mesh, vertex_layout);
}

void VertexObject::InitializeWithMeshAndLayout(const Mesh3D& mesh, const VerticesDescription& vertex_layout, unsigned int indicesCount, const void* indicesData) noexcept
{
	switch (mesh.GetShapePattern())
	{
	case ShapePattern::Line:
		pattern = GL_LINES;
		break;
	case ShapePattern::Quads:
		pattern = GL_QUADS;
		break;
	case ShapePattern::Triangles:
		pattern = GL_TRIANGLES;
		break;
	case ShapePattern::TriangleFan:
		pattern = GL_TRIANGLE_FAN;
		break;
	case ShapePattern::TriangleStrip:
		pattern = GL_TRIANGLE_STRIP;
		break;
	}
	layout = vertex_layout;
	verticesCount = static_cast<int>(mesh.GetPointsCount());
	bufferVertexCapacity = static_cast<int>(verticesCount * layout.GetVertexSize());

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	SelectVAO(*this);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, bufferVertexCapacity, NULL, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(unsigned int), indicesData, GL_STATIC_DRAW);

	layout.EnableAttributes();
	WriteMeshDataToVertexBuffer3D(mesh);
	SelectNothing();
}

void VertexObject::UpdateVeticesFromMesh(const Mesh& mesh)
{
	if (static_cast<int>(mesh.GetPointsCount()) > bufferVertexCapacity)
	{
		DeleteVerticesOnGPU();
		//InitializeWithMeshAndLayout(mesh, layout);
	}
}

void VertexObject::SelectVAO(const VertexObject& vertices) noexcept
{
	glBindVertexArray(vertices.VAO);
}

void VertexObject::SelectNothing()
{
	glBindVertexArray(0);
}

unsigned VertexObject::GetPattern() const noexcept
{
	return pattern;
}

int VertexObject::GetVerticesCount() const noexcept
{
	return verticesCount;
}

void VertexObject::WriteMeshDataToVertexBuffer3D(const Mesh3D& mesh) const noexcept
{
	char* buffer = reinterpret_cast<char*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	unsigned offset = 0;

	vec3<float> point;
	vec3<float> normal;
	vec2<float> texCoord;
	Color4f		color;

	for (int i = 0; i < static_cast<int>(verticesCount); ++i)
	{
		for (VerticesDescription::Type element : layout.GetTypes())
		{
			switch (element)
			{
				case VerticesDescription::Type::Position:
					point = mesh.GetPoint(i);
					memcpy(buffer + offset, &point, sizeof(point));
					offset += sizeof(point);
					break;
				case VerticesDescription::Type::Normal:
					normal = mesh.GetNormal(i);
					memcpy(buffer + offset, &normal, sizeof(normal));
					offset += sizeof(normal);
					break;
				case VerticesDescription::Type::TextureCoordinate:
					texCoord = mesh.GetTextureCoordinate(i);
					memcpy(buffer + offset, &texCoord, sizeof(texCoord));
					offset += sizeof(texCoord);
					break;
				case VerticesDescription::Type::Color:
					color = mesh.GetColor(i);
					memcpy(buffer + offset, &color, sizeof(color));
					offset += sizeof(Color4f);
					break;
			}
		}
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	SelectNothing();
}

void VertexObject::DeleteVerticesOnGPU() const
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void VertexObject::SetupMesh(int /*numVertices*/, int numIndices, const void* verticesData, const void* indicesData, unsigned int vertexSize)
{
	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	//unsigned int vertexSize = layout.GetVertexSize();
	glBufferData(GL_ARRAY_BUFFER, vertexSize, verticesData, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indicesData, GL_STATIC_DRAW);

	unsigned int offset = 0;
	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)0);
	offset += sizeof(vec3<float>);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)offset);
	offset += sizeof(vec3<float>);

	//// vertex texture coords
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, (void*)offsetof(Vertex, TexCoords));

	//// vertex tangent
	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

	//// vertex bitangent
	//glEnableVertexAttribArray(4);
	//glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	//// ids
	//glEnableVertexAttribArray(5);
	//glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

	//// weights
	//glEnableVertexAttribArray(6);
	//glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));

	glBindVertexArray(0);
}
