#include "GameObject.h"
#include "Vertices.h"
#include "Draw.hpp"
#include "Mesh3D.h"
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
	object->vertexObject.InitializeWithMeshAndLayout(object->meshes[0], layout);

	return object;
}

GameObject* GameObject::LoadMeshFromFBX(const std::string& filePath)
{
	unsigned int flag = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
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
	//object->meshes = new Mesh3D[object->numMeshes];
	object->meshes = new Mesh3D;

	//for (unsigned int i = 0; i < scene->mMetaData->mNumProperties; ++i)
	//{
	//	std::string s = scene->mMetaData->mKeys[i].C_Str();
	//	std::cout << s << std::endl;
	//}

	//aiString		key = scene->mMetaData->mKeys[0];
	//aiMetadataEntry entry = scene->mMetaData->mValues[0];
	//std::cout << key.C_Str() << std::endl;
	//std::cout << *(int*)entry.mData << std::endl << std::endl;

	//key = scene->mMetaData->mKeys[2];
	//entry = scene->mMetaData->mValues[2];
	//std::cout << key.C_Str() << std::endl;
	//std::cout << *(int*)entry.mData << std::endl;

	//key = scene->mMetaData->mKeys[4];
	//entry = scene->mMetaData->mValues[4];
	//std::cout << key.C_Str() << std::endl;
	//std::cout << *(int*)entry.mData << std::endl;


	aiString		key = scene->mMetaData->mKeys[8];
	aiMetadataEntry entry = scene->mMetaData->mValues[8];
	std::cout << key.C_Str() << std::endl;
	std::cout << entry.mType << std::endl;
	aiVector3D v = *(aiVector3D*)(entry.mData);
	std::cout << v.x << ", " << v.y << ", " << v.z << std::endl;
	//std::cout << *(int*)entry.mData << std::endl << std::endl;


	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
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
	}

	VerticesDescription layout = { VerticesDescription::Type::Position, VerticesDescription::Type::Normal };
	object->vertexObject.InitializeWithMeshAndLayout(*object->meshes, layout);

	// aiPrimitiveType::aiPrimitiveType_POINT
	//aiPrimitiveType::aiPrimitiveType_POINT = 0x1,
	//aiPrimitiveType_LINE = 0x2,
	//aiPrimitiveType_TRIANGLE = 0x4,
	//aiPrimitiveType_POLYGON = 0x8,

	return object;
}

void GameObject::Draw()
{
	VertexObject::SelectVAO(vertexObject);
	glDrawArrays(vertexObject.GetPattern(), 0, vertexObject.GetVerticesCount());
}
