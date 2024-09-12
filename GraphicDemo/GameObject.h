#pragma once
#include "Transform.hpp"
#include "Texture.hpp"
#include "Mesh3D.h"
#include "Draw.hpp"

enum class ObjectType
{
	JustMesh,
	HasTexture
};

class GameObject
{
public:
	GameObject() = default;
	GameObject(const vec3<float>& location,
		const vec3<float>& rotation, float scale);
	~GameObject();

	// void Initialize(Mesh newMesh, VertexObject* newVertices);

	// inline Material* GetMaterial() noexcept { return &material; }
	inline Transform* GetTransform() noexcept { return &transform; }

	void Move(const vec3<float>& v) noexcept { transform.SetTranslation(transform.GetTranslation() + v); }
	void Rotate(const vec3<float>& v) noexcept { transform.SetRotation(transform.GetRotation() + v); }

	// void SetShader(Shader* newShader) noexcept { material.shader = newShader; }
	// void SetNDC(mat4<float> ndc) noexcept { material.ndc = ndc * transform.GetModelToWorld(); }
	void SetMesh(Mesh3D* const newMesh) noexcept { meshes = newMesh; }
	void SetTexture(const Texture& newTexture) noexcept { texture = newTexture; }
	void LoadTexture(const std::filesystem::path& path) noexcept;

	mat4<float> GetModelToWorld() { return transform.BuildModelMatrix(); }

	void Draw();

	static GameObject* CreateCube(const vec3<float>& location,
		const vec3<float>& rotation, float size, Color4f color);

	static GameObject* LoadMeshFromFBX(const std::string& filePath);

private:
	Mesh3D*		 meshes;
	size_t		 numMeshes;
	VertexObject vertexObject;
	Transform	 transform;
	Texture		 texture;

	ObjectType objectType;
	// Material material;
};
