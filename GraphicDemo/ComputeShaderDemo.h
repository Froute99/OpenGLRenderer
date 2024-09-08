#pragma once
#include "Demo.hpp"
#include "Shader.h"
#include "Texture.hpp"
#include "Vertices.h"
#include "ComputeShader.h"

class ComputeShaderDemo : public Demo
{
public:
	explicit ComputeShaderDemo(OpenGLWindow& window) : Demo(window) { Initialize(); }

	void Initialize() override final;
	void Update(float dt) override final;

	void ResetCamera() override final {}

	void HandleResizeEvent(const int& new_width, const int& new_height) override final;
	//void HandleKeyPress(KeyboardButton button) override final;
	//void HandleKeyRelease(KeyboardButton button) override final;
	//void HandleScrollEvent(float scroll_amount) override final;
	void HandleFocusEvent(bool focused) override final;

	void SetValues(float* values);
	std::vector<float> GetValues();
	void PrintValues();

private:
	ComputeShader computeShader;

	vec2<unsigned int> workSize;
	std::vector<float> data;


	//VerticesDescription layout{ VerticesDescription::Type::Point, VerticesDescription::Type::TextureCoordinate };
	//Texture texture;
	//Shader textureShader;
	//VertexObject computeShaderVertices;

};

