/*
 *	Author: JeongHak Kim
 *	File_name: EnvironmentMap.cpp
 *
 *	Diffuse Irradiance, IBL
 *
 *	2025. 04. 28
 */

#pragma once
#include <Math/mat4.hpp>
#include <Math/Angle.hpp>
#include <Graphics/Shader.h>

class Mesh3D;
class VertexObject;

class EnvironmentMap
{
public:
	EnvironmentMap() = default;
	EnvironmentMap(const int screenWidth, const int screenHeight)
		: screenWidth(screenWidth), screenHeight(screenHeight) {}

	void InitFrameBuffer();
	bool LoadHDRTexture(const char* path);
	void ConvertToCubemap();
	void BakeIrradianceMap();
	void BakePrefilterMap();
	void BakeBRDFLookupTable();
	void InitSkybox(const mat4<float>& projection);

	bool CanLoad(const char* path, const mat4<float>& projection);
	void Render(const mat4<float>& view, const mat4<float>& projection);

	unsigned int GetIrradianceMapHandle() const noexcept { return irradianceMap; }
	unsigned int GetPrefilterMapHandle() const noexcept { return prefilterMap; }
	unsigned int GetBRDFLUTTextureHandle() const noexcept { return brdfLUTTexture; }

	void RenderCube();
	void RenderQuad();

	void BindIBLTexture(Shader* shader);

private:
	int screenWidth, screenHeight;

	mat4<float> captureProjection = Matrix4::GeneralProjectionMatrix(ANGLE::pi / 2.0f, 1.0f, 0.1f, 1000.f);
	mat4<float> captureViews[6] = {
		Matrix4::BuildLookAt(vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(1.0f, 0.0f, 0.0f), vec3<float>(0.0f, -1.0f, 0.0f)),
		Matrix4::BuildLookAt(vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(-1.0f, 0.0f, 0.0f), vec3<float>(0.0f, -1.0f, 0.0f)),
		Matrix4::BuildLookAt(vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(0.0f, 1.0f, 0.0f), vec3<float>(0.0f, 0.0f, 1.0f)),
		Matrix4::BuildLookAt(vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(0.0f, -1.0f, 0.0f), vec3<float>(0.0f, 0.0f, -1.0f)),
		Matrix4::BuildLookAt(vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(0.0f, 0.0f, 1.0f), vec3<float>(0.0f, -1.0f, 0.0f)),
		Matrix4::BuildLookAt(vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(0.0f, 0.0f, -1.0f), vec3<float>(0.0f, -1.0f, 0.0f))
	};

	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;

	unsigned int quadVAO = 0;
	unsigned int quadVBO = 0;

	unsigned int FBO = 0;
	unsigned int RBO = 0;

	unsigned int texture = 0;
	unsigned int cubeMap = 0;
	unsigned int irradianceMap = 0;
	unsigned int prefilterMap = 0;
	unsigned int brdfLUTTexture = 0;

	//Shader skyboxShader;
	//Mesh3D* mesh;
	//VertexObject vo;

};
