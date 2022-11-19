/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Vertices.cpp
 *	
 *	Vertices class that manage vertices
 *	
 *	Fall 2019
 *	Oct.31 2019
 */

#include "GL/glew.h"
#include "Vertices.h"
#include "Mesh.h"

Vertices::Vertices(const Mesh& mesh, const VerticesDescription& vertex_layout) noexcept
{
	InitializeWithMeshAndLayout(mesh, vertex_layout);
}

void Vertices::InitializeWithMeshAndLayout(const Mesh& mesh, const VerticesDescription& vertex_layout) noexcept
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
	SelectVAO(*this);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, bufferVertexCapacity, NULL, GL_STATIC_DRAW);

	layout.EnableAttributes();
	WriteMeshDataToVertexBuffer(mesh);
}

void Vertices::UpdateVeticesFromMesh(const Mesh& mesh)
{
	if (static_cast<int>(mesh.GetPointsCount()) > bufferVertexCapacity)
	{
		DeleteVerticesOnGPU();
		InitializeWithMeshAndLayout(mesh, layout);
	}
}

void Vertices::SelectVAO(const Vertices& vertices) noexcept
{
	glBindVertexArray(vertices.VAO);
}

void Vertices::SelectNothing()
{
	glBindVertexArray(0);
}

unsigned Vertices::GetPattern() const noexcept
{
	return pattern;
}

int Vertices::GetVerticesCount() const noexcept
{
	return verticesCount;
}

void Vertices::WriteMeshDataToVertexBuffer(const Mesh& mesh) const noexcept
{
	char* buffer = reinterpret_cast<char*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	unsigned offset = 0;

	vec2<float> point;
	Color4f color;
	vec2<float> texture;

	for (int i = 0; i < static_cast<int>(verticesCount); ++i)
	{
		for (VerticesDescription::Type element : layout.GetTypes())
		{
			switch (element)
			{
			case VerticesDescription::Type::Point:
				point = mesh.GetPoint(i);
				memcpy(buffer + offset, &point, sizeof(point));
				offset += sizeof(vec2<float>);
				break;
			case VerticesDescription::Type::Color:
				color = mesh.GetColor(i);
				memcpy(buffer + offset, &color, sizeof(color));
				offset += sizeof(Color4f);
				break;
			case VerticesDescription::Type::TextureCoordinate:
				texture = mesh.GetTextureCoordinate(i);
				memcpy(buffer + offset, &texture, sizeof(texture));
				offset += sizeof(vec2<float>);
				break;
			}
		}
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	SelectNothing();
}

void Vertices::DeleteVerticesOnGPU() const
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}
