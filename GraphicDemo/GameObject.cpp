#include "GameObject.h"
#include <Graphics/Vertices.h>
#include <Graphics/Draw.hpp>
#include <Graphics/Mesh3D.h>
#include <Math/vec3.hpp>
#include <Math/mat4.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <iostream>
#include <glew.h>

GameObject::GameObject(const vec3<float>& location, const vec3<float>& rotation, float scale)
{
	transform.SetTranslation(location);
	transform.SetRotation(rotation);
	transform.SetScale(scale);
}

GameObject::~GameObject()
{
}

void GameObject::LoadTexture(const std::filesystem::path& path) noexcept
{
	texture.LoadFromPath(path);
}

GameObject* GameObject::CreateCube(const vec3<float>& location, const vec3<float>& rotation, float size, Color4f color)
{
	GameObject* object = new GameObject(location, rotation, size);

	object->meshes = MESH::BuildCube(size, color);
	VerticesDescription layout = { VerticesDescription::Type::Position, VerticesDescription::Type::Normal };
	object->vertexObject = new VertexObject[0];
	object->vertexObject[0].InitializeWithMeshAndLayout(object->meshes[0], layout);

	return object;
}

GameObject* GameObject::LoadMeshFromFBX(const std::string& filePath)
{
	unsigned int	 flag = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
	Assimp::Importer importer;

	const aiScene*	   scene = importer.ReadFile(filePath, flag);
	const unsigned int isFailedToLoad = scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE;
	if (isFailedToLoad)
	{
		const char* errMsg = importer.GetErrorString();
		std::cout << "Error-Assimp: " << errMsg << std::endl;
	}

	GameObject* object = new GameObject();
	object->numMeshes = scene->mNumMeshes;
	object->meshes = new Mesh3D[object->numMeshes];
	object->vertexObject = new VertexObject();

	VerticesDescription layout = { VerticesDescription::Type::Position, VerticesDescription::Type::Normal };
	for (unsigned int i = 0; i < object->numMeshes; ++i)
	{
		aiMesh* aiMesh = scene->mMeshes[i];
		for (unsigned int j = 0; j < aiMesh->mNumVertices; ++j)
		{
			vec3<float> position{ aiMesh->mVertices[j].x, aiMesh->mVertices[j].z, aiMesh->mVertices[j].y };
			vec3<float> normal{ aiMesh->mNormals[j].x, aiMesh->mNormals[j].z, aiMesh->mNormals[j].y };
			object->meshes[i].AddPoint(position);
			object->meshes[i].AddNormal(normal);

			switch (aiMesh->mPrimitiveTypes)
			{
				case aiPrimitiveType::aiPrimitiveType_LINE:
					object->meshes[i].SetShapePattern(ShapePattern::Line);
					break;
				case aiPrimitiveType::aiPrimitiveType_TRIANGLE:
					object->meshes[i].SetShapePattern(ShapePattern::Triangles);
					break;
				case aiPrimitiveType::aiPrimitiveType_POLYGON:
					object->meshes[i].SetShapePattern(ShapePattern::Quads);
					break;
				default:
					object->meshes[i].SetShapePattern(ShapePattern::Quads);
					break;
			}
		}
		object->vertexObject->InitializeWithMeshAndLayout(object->meshes[i], layout);
		//object->vertexObject.InitializeWithMeshAndLayout(object->meshes[i], layout);
	}

	// aiPrimitiveType::aiPrimitiveType_POINT
	// aiPrimitiveType::aiPrimitiveType_POINT = 0x1,
	// aiPrimitiveType_LINE = 0x2,
	// aiPrimitiveType_TRIANGLE = 0x4,
	// aiPrimitiveType_POLYGON = 0x8,

	return object;
}

void GameObject::Draw()
{
	//VertexObject::SelectVAO(*vertexObject);
	//glDrawArrays(vertexObject->GetPattern(), 0, vertexObject->GetVerticesCount());
	for (size_t i = 0; i < numMeshes; ++i)
	{
		VertexObject::SelectVAO(*vertexObject);
		glDrawArrays(vertexObject->GetPattern(), 0, vertexObject->GetVerticesCount());
	}
}
