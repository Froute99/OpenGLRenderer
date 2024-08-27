#include "GameObject.h"
#include "Vertices.h"
#include "Draw.hpp"

GameObject::GameObject(Mesh newMesh, const VerticesDescription& layout)
{
	mesh = newMesh;
	material.vertices.InitializeWithMeshAndLayout(mesh, layout);

}

void GameObject::LoadTexture(const std::filesystem::path& path) noexcept
{
	material.texture.LoadFromPath(path);
}

