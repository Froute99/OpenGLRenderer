/*
 *	Author: JeongHak Kim
 *	File_name: StressTestStage.cpp
 *
 *	Stress Test
 *  Create multiple-randomized material
 *  and test with multiple objects that applied the materials
 *  
 *  Profiling performance between the batch-rendering one and not one
 *
 *	2026. 03. 03
 */

#include "StressTestStage.h"
#include <ShaderManager.h>		// access to shader instance
#include "SceneObject.h"
#include <iostream>				// printing fps

// need frame counter
// create mass of objects

void StressTestStage::Initialize()
{
	// vsync off

	Shader* pbrShader = ShaderManager::GetShader(ShaderDefinition::PBR);
	if (!envMap.CanLoad("../assets/newport_loft.hdr", view.BuildProjectionMatrix()))
	{
		std::cout << "Failed to load env map\n";
	}

	const int MAX_OBJECT_COUNT = 1;
	for (int i = 0; i < MAX_OBJECT_COUNT; ++i)
	{
		SceneObject* object = SceneObject::CreateSphere({ 0, 0, 0 });
		object->SetObjectType(ObjectType::NonTextured);
		object->Move({ 0, 0, 2.f * i });
		objects.push_back(object);
	}

}

void StressTestStage::Update(float dt)
{
	// frame count
	frameTime += dt;
	++frameCount;
	if (frameTime >= 1.0f)
	{
		int fps = (int)(frameCount / frameTime);
		frameTime -= 1.0f;
		frameCount = 0;
		std::cout << fps << "\r";
	}

	// TODO: make a non-batch drawing sequence
	
	// below is a pseudo-code of batch drawing sequence
	// Renderqueue q;
	// for (each object)
	// {
	// create render command
	// push it to render queue
	// }
	// q.sort, draw, clear

	Shader* pbrShader = ShaderManager::GetShader(ShaderDefinition::PBR);
	pbrShader->Use();
	envMap.BindIBLTexture(pbrShader);

	vec3<float> sphereColor{ 1.f, 0.f, 0.f };
	float roughness = 0.1f;
	float ambientOcclusion = 0.1f;
	float metallic = 0.8f;

	// TODO: these should go be the material
	pbrShader->SendUniformVariable("albedo", sphereColor);
	pbrShader->SendUniformVariable("roughness", roughness);
	pbrShader->SendUniformVariable("ao", ambientOcclusion);
	pbrShader->SendUniformVariable("metallic", metallic);

	for (Object* object : objects)
	{
		pbrShader->SendUniformVariable("model", object->GetModelToWorld());
		object->Draw();
	}
}

void StressTestStage::DrawGUI()
{
}
