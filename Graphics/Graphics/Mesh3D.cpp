#include "Mesh3D.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>

Mesh3D MESH::LoadFromFBX(const std::string& filePath)
{
	unsigned int flag = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filePath, flag);
	const unsigned int isFailedToLoad = scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE;
	if (isFailedToLoad)
	{
		const char* errMsg = importer.GetErrorString();
		std::cout << "Error-Assimp: " << errMsg << std::endl;
		return Mesh3D();
	}

	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* aiMesh = scene->mMeshes[i];
		for (unsigned int j = 0; j < aiMesh->mNumVertices; ++j)
		{
			std::cout << j << ": "
					  << aiMesh->mVertices[j].x << " "
					  << aiMesh->mVertices[j].y << " "
					  << aiMesh->mVertices[j].z << std::endl;
		}
	}

	return Mesh3D();
}

Mesh3D* MESH::BuildCube(float size, vec3<float> color)
{
	return BuildCube(size, Color4f(color.x, color.y, color.z, 1.f));
}

Mesh3D* MESH::BuildCube(float size, Color4f /*color*/)
{
	// counter-clockwise, thumb direction is the forward
	// this cube supposed as the front face(near plane of the cube in your thinking!)
	// is configured with four vertices. Left upper one is v0, counter clockwise v1, v2, v3
	// parrelel to the +z direction, v4, v5, v6, v7
	// with 4 vertices, one plane of the cube is consists, i need 24 vertices for total.

	vec3<float> center;

	Mesh3D* cube = new Mesh3D();
	cube->SetShapePattern(ShapePattern::Quads);

	float halfLength = size / 2.f;
	float r = center.x + halfLength; // right
	float l = center.x - halfLength; // left
	float t = center.y + halfLength; // top
	float b = center.y - halfLength; // bottom
	float p = center.z + halfLength; // positive (bigger z coordinate)
	float n = center.z - halfLength; // negative (smaller z coordinate)

	vec3<float> v0 = { r, t, n };
	vec3<float> v1 = { l, t, n };
	vec3<float> v2 = { l, b, n };
	vec3<float> v3 = { r, b, n };

	vec3<float> v4 = { r, t, p };
	vec3<float> v5 = { l, t, p };
	vec3<float> v6 = { l, b, p };
	vec3<float> v7 = { r, b, p };

	// front face
	cube->AddPoint(v0);
	cube->AddPoint(v1);
	cube->AddPoint(v2);
	cube->AddPoint(v3);

	cube->AddNormal({0.f, 0.f, -1.f});
	cube->AddNormal({0.f, 0.f, -1.f});
	cube->AddNormal({0.f, 0.f, -1.f});
	cube->AddNormal({0.f, 0.f, -1.f});

	// right face
	cube->AddPoint(v4);
	cube->AddPoint(v0);
	cube->AddPoint(v3);
	cube->AddPoint(v7);

	cube->AddNormal({ 1.f, 0.f, 0.f });
	cube->AddNormal({ 1.f, 0.f, 0.f });
	cube->AddNormal({ 1.f, 0.f, 0.f });
	cube->AddNormal({ 1.f, 0.f, 0.f });

	// up face
	cube->AddPoint(v0);
	cube->AddPoint(v4);
	cube->AddPoint(v5);
	cube->AddPoint(v1);

	cube->AddNormal({ 0.f, 1.f, 0.f });
	cube->AddNormal({ 0.f, 1.f, 0.f });
	cube->AddNormal({ 0.f, 1.f, 0.f });
	cube->AddNormal({ 0.f, 1.f, 0.f });

	// left face
	cube->AddPoint(v1);
	cube->AddPoint(v5);
	cube->AddPoint(v6);
	cube->AddPoint(v2);

	cube->AddNormal({ -1.f, 0.f, 0.f });
	cube->AddNormal({ -1.f, 0.f, 0.f });
	cube->AddNormal({ -1.f, 0.f, 0.f });
	cube->AddNormal({ -1.f, 0.f, 0.f });

	// bottom face
	cube->AddPoint(v2);
	cube->AddPoint(v6);
	cube->AddPoint(v7);
	cube->AddPoint(v3);

	cube->AddNormal({ 0.f, -1.f, 0.f });
	cube->AddNormal({ 0.f, -1.f, 0.f });
	cube->AddNormal({ 0.f, -1.f, 0.f });
	cube->AddNormal({ 0.f, -1.f, 0.f });

	// back face
	cube->AddPoint(v5);
	cube->AddPoint(v4);
	cube->AddPoint(v7);
	cube->AddPoint(v6);

	cube->AddNormal({ 0.f, 0.f, 1.f });
	cube->AddNormal({ 0.f, 0.f, 1.f });
	cube->AddNormal({ 0.f, 0.f, 1.f });
	cube->AddNormal({ 0.f, 0.f, 1.f });

	return cube;
}

