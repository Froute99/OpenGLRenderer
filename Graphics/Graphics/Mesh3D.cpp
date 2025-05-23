#include "Mesh3D.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>
#include <math/Angle.hpp>

Mesh3D MESH::LoadFromFBX(const std::string& filePath)
{
	unsigned int flag = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;

	Assimp::Importer importer;

	const aiScene*	   scene = importer.ReadFile(filePath, flag);
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
	// right-handed coordinate system. CCW winding.
	// In the right-handed coordinate system, -Z is far from the screen.

	Mesh3D* cube = new Mesh3D();

	float		halfLength = size / 2.f;
	vec3<float> v0(+halfLength, +halfLength, +halfLength);
	vec3<float> v1(-halfLength, +halfLength, +halfLength);
	vec3<float> v2(-halfLength, -halfLength, +halfLength);
	vec3<float> v3(+halfLength, -halfLength, +halfLength);

	vec3<float> v4(+halfLength, +halfLength, -halfLength);
	vec3<float> v5(-halfLength, +halfLength, -halfLength);
	vec3<float> v6(-halfLength, -halfLength, -halfLength);
	vec3<float> v7(+halfLength, -halfLength, -halfLength);

	// front face
	cube->AddPoint(v0);
	cube->AddPoint(v1);
	cube->AddPoint(v2);
	cube->AddPoint(v3);

	cube->AddNormal({ 0.f, 0.f, 1.f });
	cube->AddNormal({ 0.f, 0.f, 1.f });
	cube->AddNormal({ 0.f, 0.f, 1.f });
	cube->AddNormal({ 0.f, 0.f, 1.f });

	cube->AddIndices({ 0, 1, 2 });
	cube->AddIndices({ 2, 3, 0 });

	// right face
	cube->AddPoint(v4);
	cube->AddPoint(v0);
	cube->AddPoint(v3);
	cube->AddPoint(v7);

	cube->AddNormal({ 1.f, 0.f, 0.f });
	cube->AddNormal({ 1.f, 0.f, 0.f });
	cube->AddNormal({ 1.f, 0.f, 0.f });
	cube->AddNormal({ 1.f, 0.f, 0.f });

	cube->AddIndices({ 4, 5, 6 });
	cube->AddIndices({ 6, 7, 4 });

	// up face
	cube->AddPoint(v4);
	cube->AddPoint(v5);
	cube->AddPoint(v1);
	cube->AddPoint(v0);

	cube->AddNormal({ 0.f, 1.f, 0.f });
	cube->AddNormal({ 0.f, 1.f, 0.f });
	cube->AddNormal({ 0.f, 1.f, 0.f });
	cube->AddNormal({ 0.f, 1.f, 0.f });

	cube->AddIndices({ 8, 9, 10 });
	cube->AddIndices({ 10, 11, 8 });

	// left face
	cube->AddPoint(v1);
	cube->AddPoint(v5);
	cube->AddPoint(v6);
	cube->AddPoint(v2);

	cube->AddNormal({ -1.f, 0.f, 0.f });
	cube->AddNormal({ -1.f, 0.f, 0.f });
	cube->AddNormal({ -1.f, 0.f, 0.f });
	cube->AddNormal({ -1.f, 0.f, 0.f });

	cube->AddIndices({ 12, 13, 14 });
	cube->AddIndices({ 14, 15, 12 });

	// bottom face
	cube->AddPoint(v3); // r, b, n
	cube->AddPoint(v2); // l, b, n
	cube->AddPoint(v6); // l, b, f
	cube->AddPoint(v7); // r, b, f

	cube->AddNormal({ 0.f, -1.f, 0.f });
	cube->AddNormal({ 0.f, -1.f, 0.f });
	cube->AddNormal({ 0.f, -1.f, 0.f });
	cube->AddNormal({ 0.f, -1.f, 0.f });

	cube->AddIndices({ 16, 17, 18 });
	cube->AddIndices({ 18, 19, 16 });

	// back face
	cube->AddPoint(v5);
	cube->AddPoint(v4);
	cube->AddPoint(v7);
	cube->AddPoint(v6);

	cube->AddNormal({ 0.f, 0.f, -1.f });
	cube->AddNormal({ 0.f, 0.f, -1.f });
	cube->AddNormal({ 0.f, 0.f, -1.f });
	cube->AddNormal({ 0.f, 0.f, -1.f });

	cube->AddIndices({ 20, 21, 22 });
	cube->AddIndices({ 22, 23, 20 });

	return cube;
}

/* This method create sphere by merging two hemisphere
 * Hemispheres which look from top(eye +Y to -Y)
 * Each loop makes a circle has vertices of SEGMENTS
 *
 * Indices consist like this:
 * (in 16 16 segments sphere)
 * 0    *    17
 * 1   ***   18
 *    *****
 * 2 ******* 19
 *
 * Top Right Left = CCW winding
 * 0 1 17, 17 1 18
 * 1 2 18, 18 2 19
 *
 * So, this sphere drawing
 */
Mesh3D* MESH::BuildSphere()
{

	Mesh3D* sphere = new Mesh3D();
	sphere->SetShapePattern(ShapePattern::TriangleStrip);

	const unsigned int X_SEGMENTS = 32;
	const unsigned int Y_SEGMENTS = 32;

	for (unsigned int i = 0; i <= X_SEGMENTS; ++i)
	{
		for (unsigned int j = 0; j <= Y_SEGMENTS; ++j)
		{
			float xSegment = (float)i / (float)X_SEGMENTS;
			float ySegment = (float)j / (float)Y_SEGMENTS;
			float x = std::cos(xSegment * ANGLE::two_pi) * std::sin(ySegment * ANGLE::pi);
			float y = std::cos(ySegment * ANGLE::pi);
			float z = std::sin(xSegment * ANGLE::two_pi) * std::sin(ySegment * ANGLE::pi);

			sphere->AddPoint({ x, y, z });
			sphere->AddNormal({ x, y, z });
			sphere->AddTexCoord({ xSegment, ySegment });
		}
	}

	// Indices
	bool oddRow = false;
	for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
	{
		if (!oddRow)
		{
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
			{
				sphere->AddIndex(y * (X_SEGMENTS + 1) + x);
				sphere->AddIndex((y + 1) * (X_SEGMENTS + 1) + x);
			}
		}
		else
		{
			for (int x = X_SEGMENTS; x >= 0; --x)
			{
				sphere->AddIndex((y + 1) * (X_SEGMENTS + 1) + x);
				sphere->AddIndex(y * (X_SEGMENTS + 1) + x);
			}
		}
		oddRow = !oddRow;
	}

	return sphere;
}
