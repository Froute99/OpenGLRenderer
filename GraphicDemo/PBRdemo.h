
#pragma once

#include "Demo.h"
#include <Graphics/Shader.h>
#include <Graphics/Texture.h>
#include <EnvironmentMap.h>

class GameObject;

class PBRDemo : public Demo
{
public:
	explicit PBRDemo(OpenGLWindow& window)
		: Demo(window) { Initialize(); }

	void Initialize() override;
	void Update(float dt) override;
	void ResetCamera() override;

	void HandleResizeEvent(const int& new_width, const int& new_height) override final;
	void HandleKeyPress(KeyboardButton button) override final;
	void HandleKeyRelease(KeyboardButton button) override final;
	void HandleScrollEvent(float scroll_amount) override final;
	void HandleFocusEvent(bool focused) override final;

	void ImguiHelper();

private:
	vec3<float> moveSpeed;

	Shader pbrShader;

	GameObject* sphere;

	vec3<float>	 sphereColor;
	float		 roughness;
	float		 ambientOcclusion;
	float		 metallic;

	unsigned int lightPosLocation;
	unsigned int lightColLocation;
	vec3<float>	 lightPos[4];
	vec3<float>	 lightCol[4];

	Shader		 hdrShader;
	unsigned int hdrFBO;	  // framebuffer object handle for HDR
	unsigned int colorBuffer; // colorbuffer for HDR
	bool		 hdr = true;
	float		 exposure = 1.0f;

	void RenderQuad();
	unsigned int quadVAO = 0;
	unsigned int quadVBO;

	EnvironmentMap envMap{ GetScreenWidth(), GetScreenHeight() };

	bool shouldIrradiance = true;

	int				  surfaceIndex = 0;
	const char* const surfacesList[6] = { "Plastic", "Glass", "Diamond", "Iron", "Copper", "Gold" };
};
