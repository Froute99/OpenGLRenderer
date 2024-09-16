#include "ComputeShaderDemo.h"
#include <Graphics/Mesh.h>
#include <Graphics/Draw.hpp>
#include <Graphics/PATH.hpp>
#include <glew.h>
#include <iostream>

void ComputeShaderDemo::Initialize()
{
	workSize = { 10, 1 };
	computeShader.Initialize(workSize.x, workSize.y);

	data = { 0,1,2,3,4,5,6,7,8,9 };
	computeShader.Use();		// why?
	SetValues(data.data());


	//textureShader.LoadShaderFrom(PATH::texture_vert, PATH::texture_frag);
	//const Mesh& rectangle = MESH::create_rectangle({ 0.0f }, { 1.0f }, { 0.0f });
	//computeShaderVertices.InitializeWithMeshAndLayout(rectangle, layout);
	//texture.CreateTexture(workSize.x, workSize.y);

}

void ComputeShaderDemo::Update(float /*dt*/)
{
	computeShader.Run();
	PrintValues();


	//computeShader.Use();
	//computeShader.BindTexture();
	//computeShader.Dispatch(workSize.x, workSize.y, 1);

	//// don't know how this works, but it ensure image writing finished before read.
	//computeShader.Wait();
	////glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
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

void ComputeShaderDemo::PrintValues()
{
	auto computeResult = GetValues();
	for (auto element : computeResult)
	{
		std::cout << element << " ";
	}
	std::cout << std::endl;
}
