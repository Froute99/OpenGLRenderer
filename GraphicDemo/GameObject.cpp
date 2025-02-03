#include "GameObject.h"
#include <Graphics/Vertices.h>
#include <Graphics/Draw.h>
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

	return cube;
}

GameObject* GameObject::CreateSphere(const vec3<float>& location)
{
	GameObject* sphere = new GameObject(location, { 0, 0, 0 }, 1);
	Mesh3D*		sphereMesh = MESH::BuildSphere();
	VerticesDescription layout{
		VerticesDescription::Type::Position,
		VerticesDescription::Type::Normal,
		VerticesDescription::Type::TextureCoordinate
	};
	VertexObject*		vertexObject = new VertexObject(sphereMesh, layout);
	sphere->AddMesh(sphereMesh);
	sphere->vertexObject.push_back(vertexObject);
	return sphere;

	//if (sphereVAO == 0)
	//{
	//	glGenVertexArrays(1, &sphereVAO);

	//	unsigned int vbo, ebo;
	//	glGenBuffers(1, &vbo);
	//	glGenBuffers(1, &ebo);

	//	std::vector<glm::vec3>	  positions;
	//	std::vector<glm::vec2>	  uv;
	//	std::vector<glm::vec3>	  normals;
	//	std::vector<unsigned int> indices;

	//	const unsigned int X_SEGMENTS = 64;
	//	const unsigned int Y_SEGMENTS = 64;
	//	const float		   PI = 3.14159265359f;
	//	for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
	//	{
	//		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
	//		{
	//			float xSegment = (float)x / (float)X_SEGMENTS;
	//			float ySegment = (float)y / (float)Y_SEGMENTS;
	//			float xPos =
	//				std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
	//			float yPos = std::cos(ySegment * PI);
	//			float zPos =
	//				std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

	//			positions.push_back(glm::vec3(xPos, yPos, zPos));
	//			uv.push_back(glm::vec2(xSegment, ySegment));
	//			normals.push_back(glm::vec3(xPos, yPos, zPos));
	//		}
	//	}

	//	bool oddRow = false;
	//	for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
	//	{
	//		if (!oddRow) // even rows: y == 0, y == 2; and so on
	//		{
	//			for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
	//			{
	//				indices.push_back(y * (X_SEGMENTS + 1) + x);
	//				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
	//			}
	//		}
	//		else
	//		{
	//			for (int x = X_SEGMENTS; x >= 0; --x)
	//			{
	//				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
	//				indices.push_back(y * (X_SEGMENTS + 1) + x);
	//			}
	//		}
	//		oddRow = !oddRow;
	//	}
	//	indexCount = static_cast<unsigned int>(indices.size());

	//	std::vector<float> data;
	//	for (unsigned int i = 0; i < positions.size(); ++i)
	//	{
	//		data.push_back(positions[i].x);
	//		data.push_back(positions[i].y);
	//		data.push_back(positions[i].z);
	//		if (normals.size() > 0)
	//		{
	//			data.push_back(normals[i].x);
	//			data.push_back(normals[i].y);
	//			data.push_back(normals[i].z);
	//		}
	//		if (uv.size() > 0)
	//		{
	//			data.push_back(uv[i].x);
	//			data.push_back(uv[i].y);
	//		}
	//	}
	//	glBindVertexArray(sphereVAO);
	//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0],
	//		GL_STATIC_DRAW);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	//		indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	//	unsigned int stride = (3 + 2 + 3) * sizeof(float);
	//	glEnableVertexAttribArray(0);
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	//	glEnableVertexAttribArray(1);
	//	glVertexAttribPointer(
	//		1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	//	glEnableVertexAttribArray(2);
	//	glVertexAttribPointer(
	//		2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
	//}

	//glBindVertexArray(sphereVAO);
	//glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}

GameObject* GameObject::LoadMeshFromFile(const std::string& filePath)
{
	std::cout << "Start to load file: " << filePath << std::endl;
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

	std::string baseRoot = "../assets/Models/";
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

		CheckMaterialTextureTypeExist(material, aiTextureType_DIFFUSE);
		CheckMaterialTextureTypeExist(material, aiTextureType_BASE_COLOR);
		CheckMaterialTextureTypeExist(material, aiTextureType_SPECULAR);
		CheckMaterialTextureTypeExist(material, aiTextureType_METALNESS);
		CheckMaterialTextureTypeExist(material, aiTextureType_DIFFUSE_ROUGHNESS);
		CheckMaterialTextureTypeExist(material, aiTextureType_NORMALS);
		CheckMaterialTextureTypeExist(material, aiTextureType_HEIGHT);
		CheckMaterialTextureTypeExist(material, aiTextureType_AMBIENT);
		CheckMaterialTextureTypeExist(material, aiTextureType_AMBIENT_OCCLUSION);
		//std::cout << "BaseColor: " << material->GetTextureCount(aiTextureType_BASE_COLOR) << std::endl;
		//std::cout << "Diffuse: " << material->GetTextureCount(aiTextureType_DIFFUSE) << std::endl;
		//std::cout << "Specular: " << material->GetTextureCount(aiTextureType_SPECULAR) << std::endl;
		//std::cout << "Metallic: " << material->GetTextureCount(aiTextureType_METALNESS) << std::endl;
		//std::cout << "Roughness: " << material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) << std::endl;
		//std::cout << "Normal: " << material->GetTextureCount(aiTextureType_NORMALS) << std::endl;
		//std::cout << "Height: " << material->GetTextureCount(aiTextureType_HEIGHT) << std::endl;
		//std::cout << "A: " << material->GetTextureCount(aiTextureType_AMBIENT) << std::endl;
		//std::cout << "AO: " << material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION) << std::endl;
		////std::cout << "Emissive: " << material->GetTextureCount(aiTextureType_EMISSIVE) << std::endl;
		////std::cout << "Displacement: " << material->GetTextureCount(aiTextureType_DISPLACEMENT) << std::endl;
		////std::cout << "Reflection: " << material->GetTextureCount(aiTextureType_REFLECTION) << std::endl;
		////std::cout << "Transmission: " << material->GetTextureCount(aiTextureType_TRANSMISSION) << std::endl;
		////std::cout << "Shininess: " << material->GetTextureCount(aiTextureType_SHININESS) << std::endl;
		////std::cout << "LightMap: " << material->GetTextureCount(aiTextureType_LIGHTMAP) << std::endl;
		////std::cout << "CLEARCOAT: " << material->GetTextureCount(aiTextureType_CLEARCOAT) << std::endl;
		////std::cout << "NormalCamera: " << material->GetTextureCount(aiTextureType_NORMAL_CAMERA) << std::endl;
		////std::cout << "SHEEN: " << material->GetTextureCount(aiTextureType_SHEEN) << std::endl;
		////std::cout << "Opacity: " << material->GetTextureCount(aiTextureType_OPACITY) << std::endl;
		//std::cout << "Unknown: " << material->GetTextureCount(aiTextureType_UNKNOWN) << std::endl;
		//std::cout << "None: " << material->GetTextureCount(aiTextureType_NONE) << std::endl;
		

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
		// Activate and bind textures
	}
	unsigned int numMeshes = meshes.size();
	for (unsigned int i = 0; i < numMeshes; ++i)
	{
		// using below one instead of GL_TRIANGLES
		// vertexObject[i]->GetPattern();
		glBindVertexArray(vertexObject[i]->VAO);
		glDrawElements(vertexObject[i]->GetPattern(), meshes[i]->GetIndicesCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

void GameObject::CheckMaterialTextureTypeExist(aiMaterial* material, aiTextureType type)
{
	unsigned int count = material->GetTextureCount(type);
	if (count > 0)
	{
		std::cout << aiTextureTypeToString(type) << ": " << count << std::endl;
	}
}
