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
#include <vector>

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
	Texture texture;
	texture.LoadFromPath(path);
	textures.push_back(texture);
}

GameObject* GameObject::CreateCube(const vec3<float>& location, const vec3<float>& rotation, float size)
{
	GameObject* cube = new GameObject(location, rotation, size);

	Mesh3D*				cubeMesh = MESH::BuildCube(1.f);
	VerticesDescription layout{ VerticesDescription::Type::Position };
	VertexObject*		vertexObject = new VertexObject(cubeMesh, layout);
	cube->AddMesh(cubeMesh);
	cube->vertexObject.push_back(vertexObject);

	// object->meshes.push_back(MESH::BuildCube(size, color));
	// VerticesDescription layout = { VerticesDescription::Type::Position, VerticesDescription::Type::Normal };
	// VertexObject*		vobj = new VertexObject;
	// vobj->InitializeWithMeshAndLayout(*object->meshes[0], layout);
	// object->vertexObject.push_back(vobj);

	return cube;
}

GameObject* GameObject::LoadMeshFromFile(const std::string& filePath)
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
	object->meshes.reserve(scene->mNumMeshes);
	object->vertexObject.reserve(scene->mNumMeshes);

	object->ProcessNode(scene->mRootNode, scene);

	for (auto& path : object->texturePaths)
	{
		std::cout << path << std::endl;
		object->LoadTexture(path);
	}

	if (object->texturePaths.empty())
	{
		object->SetObjectType(ObjectType::NonTextured);
	}

	return object;
}

void GameObject::ProcessNode(aiNode* node, const aiScene* scene)
{
	// node->mTransformation.
	//  process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh3D* GameObject::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	Mesh3D* result = new Mesh3D();

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		result->AddPoint({ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });

		if (mesh->HasNormals())
		{
			result->AddNormal({ mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z });
		}

		if (mesh->mTextureCoords[0])
		{
			result->AddTexCoord({ mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else
		{
			result->AddTexCoord({ 0.0f, 0.0f });
		}

		switch (mesh->mPrimitiveTypes)
		{
			case aiPrimitiveType::aiPrimitiveType_LINE:
				result->SetShapePattern(ShapePattern::Line);
				break;
			case aiPrimitiveType::aiPrimitiveType_TRIANGLE:
				result->SetShapePattern(ShapePattern::Triangles);
				break;
			case aiPrimitiveType::aiPrimitiveType_POLYGON:
				result->SetShapePattern(ShapePattern::Quads);
				break;
			default:
				result->SetShapePattern(ShapePattern::Triangles);
				break;
		}
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			result->AddIndex(face.mIndices[j]);
		}
	}

	VerticesDescription layout = { VerticesDescription::Type::Position, VerticesDescription::Type::Normal, VerticesDescription::Type::TextureCoordinate };
	VertexObject*		v = new VertexObject;
	v->InitializeWithMeshAndLayout(*result, layout);
	vertexObject.push_back(v);

	std::string baseRoot = "../assets/backpack/";
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		for (unsigned int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); ++i)
		{
			aiString s;
			material->GetTexture(aiTextureType_DIFFUSE, i, &s);
			std::string texturePath = baseRoot + s.C_Str();
			texturePaths.insert(texturePath);
		}

		// for (unsigned int i = 0; i < material->GetTextureCount(aiTextureType_SPECULAR); ++i)
		//{
		//	aiString s;
		//	material->GetTexture(aiTextureType_SPECULAR, i, &s);
		//	std::string texturePath = baseRoot + s.C_Str();
		//	texturePaths.insert(texturePath);
		// }
		//// 3. normal maps
		// std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		// textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		//// 4. height maps
		// std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		// textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

	return result;
}

void GameObject::Draw()
{
	if (objectType == ObjectType::Textured)
	{
		DrawTextured();
	}
	else
	{
		DrawNonTextured();
	}
	//glActiveTexture(GL_TEXTURE0);
	////glUniform1i(glGetUniformLocation(shader.GetHandleToShader(), "textureDiffuse1"), 0);
	//glBindTexture(GL_TEXTURE_2D, textures[0].GetTexturehandle());

	//unsigned int numMeshes = meshes.size();
	//for (unsigned int i = 0; i < numMeshes; ++i)
	//{
	//	glBindVertexArray(vertexObject[i]->VAO);
	//	glDrawElements(GL_TRIANGLES, meshes[i]->GetIndicesCount(), GL_UNSIGNED_INT, 0);
	//	glBindVertexArray(0);
	//}
}

void GameObject::DrawTextured()
{
	glActiveTexture(GL_TEXTURE0);
	// glUniform1i(glGetUniformLocation(shader.GetHandleToShader(), "textureDiffuse1"), 0);
	glBindTexture(GL_TEXTURE_2D, textures[0].GetTexturehandle());

	unsigned int numMeshes = meshes.size();
	for (unsigned int i = 0; i < numMeshes; ++i)
	{
		glBindVertexArray(vertexObject[i]->VAO);
		glDrawElements(GL_TRIANGLES, meshes[i]->GetIndicesCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

void GameObject::DrawNonTextured()
{
	unsigned int numMeshes = meshes.size();
	for (unsigned int i = 0; i < numMeshes; ++i)
	{
		std::cout << "Draw Non Textured\n";
		glBindVertexArray(vertexObject[i]->VAO);
		glDrawElements(GL_TRIANGLES, meshes[i]->GetIndicesCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}
