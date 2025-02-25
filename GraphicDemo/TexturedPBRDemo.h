
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

	unsigned int uniformModelLocation;
	unsigned int uniformViewLocation;
	unsigned int uniformProjectionLocation;

	unsigned int albedoMapLocation;
	Texture*	 albedoMap;

	unsigned int normalMapLocation;
	Texture*	 normalMap;

	unsigned int roughnessMapLocation;
	Texture*	 roughnessMap;

	unsigned int aoMapLocation;
	Texture*	 aoMap;

	unsigned int metallicMapLocation;
	Texture*	 metallicMap;

	unsigned int lightPosLocation;
	unsigned int lightColLocation;
	//vec3<float>	 lightPos[4];
	//vec3<float>	 lightCol[4];
	vec3<float> lightPos;
	vec3<float> lightCol;

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

	// IMGui
	float lightIntensity = 1.f;

	Shader equirectangularMappingShader;

};
