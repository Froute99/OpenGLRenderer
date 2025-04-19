
#pragma once
#include "Demo.h"
#include <Graphics/Shader.h>
#include <Graphics/Texture.h>

class GameObject;

class TexturedPBRDemo : public Demo
{
public:
	explicit TexturedPBRDemo(OpenGLWindow& window)
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

	GameObject* backpack;
	GameObject* sphere;

	Texture* albedoMap;
	Texture* normalMap;
	Texture* roughnessMap;
	Texture* aoMap;
	Texture* metallicMap;

	vec3<float> lightPos;
	vec3<float> lightColor;

	unsigned int camPosLocation;

	unsigned int gammaLocation;
	float		 gamma = 1.0f;
	bool		 shouldGammaCorrected = false;

	Shader		 hdrShader;
	unsigned int hdrFBO;	  // framebuffer object handle for HDR
	unsigned int colorBuffer; // colorbuffer for HDR
	bool		 hdr = true;
	float		 exposure = 1.0f;

	void		 RenderQuad();
	unsigned int quadVAO = 0;
	unsigned int quadVBO;

	// IBL (Diffuse)
	Shader equirectangularMappingShader;

	vec3<float> rotationOffset{ 0.f };

};
