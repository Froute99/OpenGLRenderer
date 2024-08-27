#pragma once
#include "Transform.hpp"
#include "Texture.hpp"
#include "Mesh.h"
#include "Draw.hpp"

class GameObject
{
public:
	GameObject() = default;
	GameObject(Mesh newMesh, const VerticesDescription& layout);
	~GameObject() {}

	//void Initialize(Mesh newMesh, Vertices* newVertices);

	inline Material* GetMaterial() noexcept { return &material; }
	inline Transform* GetTransform() noexcept { return &transform; }

	void SetShader(Shader* newShader) noexcept { material.shader = newShader; }
	void SetNDC(mat3<float> ndc) noexcept { material.ndc = ndc * transform.GetModelToWorld(); }
	void SetTexture(const Texture& texture) noexcept { material.texture = texture; }
	void LoadTexture(const std::filesystem::path& path) noexcept;

private:
	float yaw;
	float pitch;
	float roll;

	// already included in Transform class
	//float x;
	//float y;
	//float z;

	Material material;
	Mesh mesh;
	Transform transform;

};
