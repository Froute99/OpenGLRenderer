#include "ComputeShaderDemo.h"
#include "Mesh.h"
#include "Draw.hpp"
#include "PATH.hpp"
#include "gl/glew.h"
#include <iostream>

void ComputeShaderDemo::Initialize()
{
	const std::filesystem::path computeShaderPath = "../assets/compute.glsl";
	computeShader.LoadShader(computeShaderPath);
	textureShader.LoadShaderFrom(PATH::texture_vert, PATH::texture_frag);

	const Mesh& rectangle = MESH::create_rectangle({ 0.0f }, { 1.0f }, { 0.0f });
	computeShaderVertices.InitializeWithMeshAndLayout(rectangle, layout);

	texture.CreateTexture(10, workSize.y);

	data = { 0,1,2,3,4,5,6,7,8,9 };
	computeShader.Use();
	SetValues(data.data());

}

void ComputeShaderDemo::Update(float /*dt*/)
{
	//Draw::StartDrawing();

	//Shader::UseShader(textureShader);
	//textureShader.SendUniformVariable("tex", 0);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture.GetTexturehandle());
	//Vertices::SelectVAO(computeShaderVertices);
	//glDrawArrays(computeShaderVertices.GetPattern(), 0, computeShaderVertices.GetVerticesCount());

	//std::vector<float> values{ 0,1,2,3,4,5,6,7,8,9 };

	computeShader.Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.GetTexturehandle());
	glDispatchCompute(workSize.x, workSize.y, 1);

	// don't know how this works, but it ensure image writing finished before read.
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	//glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	auto computeResult = GetValues();
	for (auto element : computeResult)
	{
		std::cout << element << " ";
	}
	std::cout << std::endl;

	//Draw::FinishDrawing();
}

void ComputeShaderDemo::ResetCamera()
{
}

void ComputeShaderDemo::HandleResizeEvent(const int& new_width, const int& new_height)
{
	Demo::HandleResizeEvent(new_width, new_height);
}

void ComputeShaderDemo::HandleFocusEvent(bool focused)
{
	Demo::HandleFocusEvent(focused);
}

void ComputeShaderDemo::SetValues(float* values)
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, workSize.x, workSize.y, 0, GL_RED, GL_FLOAT, values);
}

std::vector<float> ComputeShaderDemo::GetValues()
{
	size_t dataSize = workSize.x * workSize.y;
	std::vector<float> computeResult(dataSize);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, computeResult.data());
	return computeResult;
}
