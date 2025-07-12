#pragma once
#include <Math/mat4.hpp>
#include <Graphics/Shader.h>

class Mesh3D;
class VertexObject;

class EnvironmentMap
{
public:
	EnvironmentMap() = default;
	EnvironmentMap(const int screenWidth, const int screenHeight)
		: screenWidth(screenWidth), screenHeight(screenHeight) {}
	bool CanLoad(const char* path, const mat4<float>& projection);
	void Render(const mat4<float>& view, const mat4<float>& projection);

	unsigned int GetIrradianceMapHandle() const noexcept { return irradianceMap; }
	unsigned int GetPrefilterMapHandle() const noexcept { return prefilterMap; }
	unsigned int GetBRDFLUTTextureHandle() const noexcept { return brdfLUTTexture; }

	void RenderCube();
	void RenderQuad();

private:
	int screenWidth, screenHeight;

	Shader equirectangularMappingShader;
	Shader skyboxShader;
	Shader irradianceShader;
	Shader prefilterShader;
	Shader brdfShader;

	//Mesh3D* mesh;
	//VertexObject vo;

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

	//mat4<float> captureProjection;
	//mat4<float> captureViews[6];
};
