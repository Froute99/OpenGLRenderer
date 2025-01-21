
#pragma once

#include "Demo.hpp"
#include <Graphics/Shader.h>

#include <Graphics/Texture.hpp>

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

private:
	vec3<float> moveSpeed;

	Shader pbrShader;

	GameObject* sphere;

	unsigned int uniformModelLocation;
	unsigned int uniformViewLocation;
	unsigned int uniformProjectionLocation;

	unsigned int sphereColorLocation;
	vec3<float>	 sphereColor;

	unsigned int roughnessLocation;
	float		 roughness;

	unsigned int aoLocation;
	float		 ambientOcclusion;

	unsigned int metallicLocation;
	float		 metallic;

	unsigned int lightPosLocation;
	unsigned int lightColLocation;
	vec3<float>	 lightPos[4];
	vec3<float>	 lightCol[4];

	unsigned int camPosLocation;

	unsigned int gammaLocation;
	float		 gamma = 1.0f;
	bool		 shouldGammaCorrected = false;

	Shader		 hdrShader;
	unsigned int hdrFBO;	  // framebuffer object handle for HDR
	unsigned int colorBuffer; // colorbuffer for HDR
	bool		 hdr = true;
	float		 exposure = 1.0f;

	void RenderQuad();
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
};
