/********************************************************
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *
 *	File_name: ShapeDrawingDemo.h
 *
 *	Simple Shape Drawing Demo
 *
 *	Nov.25 2019
 *******************************************************/

#pragma once
#include "Demo.hpp"
#include <Graphics/Shader.h>

class Object;
class GameObject;
struct aiScene;

class ShapeDrawingDemo : public Demo
{
public:
	explicit ShapeDrawingDemo(OpenGLWindow& window)
		: Demo(window) { Initialize(); }

	void Initialize() override final;
	void Update(float dt) override final;

	void ResetCamera() override final;

	void HandleResizeEvent(const int& new_width, const int& new_height) override final;
	void HandleKeyPress(KeyboardButton button) override final;
	void HandleKeyRelease(KeyboardButton button) override final;
	void HandleScrollEvent(float scroll_amount) override final;
	void HandleFocusEvent(bool focused) override final;

	void ImguiHelper();

private:
	vec3<float> moveSpeed;

	//Object* cube;
	GameObject* backpack;
	Object* skybox;

	Shader shader;
	Shader skyboxShader;

	GLuint uniformModelLocation;
	GLuint uniformViewLocation;
	GLuint uniformProjectionLocation;

	GLuint uniformObjectColorLocation;
	GLuint uniformLightPosLocation;
	GLuint uniformLightColorLocation;

	GLuint uniformLightCubeModel;
	GLuint uniformLightCubeView;
	GLuint uniformLightCubeProjection;

	vec3<float> objectColor;
	vec3<float> lightPos;
	vec3<float> lightColor;

	vec3<float> backpackTranslation;

	float angle = 1.f;

	std::vector<std::string> cubemapPaths;
	unsigned int			 cubemapTextureID;
	unsigned int			 skyboxVAO;
	unsigned int			 skyboxVBO;
};
