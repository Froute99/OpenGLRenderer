#pragma once
#include "Object.h"					// parent class
#include <Graphics/Transform.h>	// transforms
#include <Graphics/Texture.h>		// textures
#include <Graphics/Mesh3D.h>		// meshes
#include <Graphics/Vertices.h>		// vertex object
#include <Math/mat4.hpp>			// matrices for the transforms
#include <set>						// texture paths container

struct aiScene;
struct aiNode;
struct aiMesh;

struct Vertex
{
	vec3<float> position;
	vec3<float> normal;
};

enum class ObjectType
{
	Textured,
	NonTextured
};

class GameObject : public Object
{
public:
	GameObject() = default;
	GameObject(const vec3<float>& location,
		const vec3<float>& rotation, float scale);
	~GameObject();

	// void Initialize(Mesh newMesh, VertexObject* newVertices);

	// inline Material* GetMaterial() noexcept { return &material; }
	//inline Transform* GetTransform() noexcept { return &transform; }

	//void Move(const vec3<float>& v) noexcept { transform.SetTranslation(transform.GetTranslation() + v); }
	//void Rotate(const vec3<float>& v) noexcept { transform.SetRotation(transform.GetRotation() + v); }

	// void SetShader(Shader* newShader) noexcept { material.shader = newShader; }
	// void SetNDC(mat4<float> ndc) noexcept { material.ndc = ndc * transform.GetModelToWorld(); }
	void AddMesh(Mesh3D* const newMesh) noexcept { meshes.push_back(newMesh); }
	// void SetTexture(const Texture& newTexture) noexcept { texture = newTexture; }
	void LoadTexture(const std::filesystem::path& path) noexcept;

	//mat4<float> GetModelToWorld() { return transform.BuildModelMatrix(); }

	virtual void Draw() override;

	static GameObject* CreateCube(const vec3<float>& location,
		const vec3<float>& rotation, float size);

	static GameObject* LoadMeshFromFile(const std::string& filePath);
	void			   ProcessNode(aiNode* node, const aiScene* scene);
	Mesh3D*			   ProcessMesh(aiMesh* mesh, const aiScene* scene);

	// For Debug
	unsigned int GetTextureHandle(int index) { return textures[index].GetTexturehandle(); }

	ObjectType GetObjectType() { return objectType; }
	void	   SetObjectType(ObjectType newObjectType) { objectType = newObjectType; }

private:
	void DrawTextured();
	void DrawNonTextured();

	ObjectType objectType = ObjectType::Textured;

private:
	std::vector<Mesh3D*> meshes;

	std::vector<VertexObject*> vertexObject;

	std::set<std::string> texturePaths;
	std::vector<Texture>  textures;

	Transform  transform;
	// Material material;
};
