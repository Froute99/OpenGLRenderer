#include "GameObject.h"
#include "Vertices.h"
#include "Draw.hpp"
#include "Mesh3D.h"

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

	mesh = MESH::BuildCube(transform.GetTranslation(), size, color);
	VerticesDescription layout = { VerticesDescription::Type::Position, VerticesDescription::Type::Normal };
	vertices.InitializeWithMeshAndLayout(mesh, layout);

	return object;
}

