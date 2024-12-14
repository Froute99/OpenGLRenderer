
#pragma once

#include "Demo.hpp"
#include <Graphics/Shader.h>

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
};
