#include "GameObject.h"
#include "Vertices.h"
#include "Draw.hpp"
#include "Mesh3D.h"
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

	object->mesh = MESH::BuildCube(size, color);
	VerticesDescription layout = { VerticesDescription::Type::Position, VerticesDescription::Type::Normal };
	object->vertexObject.InitializeWithMeshAndLayout(object->mesh, layout);

	return object;
}

void GameObject::Draw()
{
	VertexObject::SelectVAO(vertexObject);
	glDrawArrays(vertexObject.GetPattern(), 0, vertexObject.GetVerticesCount());
}
