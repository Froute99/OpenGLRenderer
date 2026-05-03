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
#include "SceneObject.h"
#include <ShaderManager.h>		// access to shader instance
#include <iostream>				// printing fps

// imguis
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Material.h"

#include <random>

void StressTestStage::Initialize()
{
	// TODO: vsync off

	Shader* pbrShader = ShaderManager::GetShader(ShaderDefinition::PBR);
	if (!envMap.CanLoad("../assets/newport_loft.hdr", view.BuildProjectionMatrix()))
	{
		std::cout << "Failed to load env map\n";
	}

	GlobalUniformManager& uniformManager = GlobalUniformManager::GetInstance();
	uniformManager.Init(pbrShader->GetHandleToShader());
	uniformManager.SetLightPosition({ 3.6f, 0.55f, -0.4f });
	uniformManager.SetLightColor({ 1.f });
	uniformManager.SetLightIntensity(200.f);

	// 500 meshes, 20 materials 500*20 = 10000
	const int MAX_OBJECT_COUNT = 10000;
	const int MAX_MATERIAL_COUNT = 1;

	std::random_device					  rd;
	std::mt19937						  ren(rd());		// random engine
	std::uniform_real_distribution<float> fd(0.f, 1.f);		// float distribution
	stockMaterials.reserve(MAX_MATERIAL_COUNT);
	for (int i = 0; i < MAX_MATERIAL_COUNT; ++i)
	{
		SimpleMaterialPBR* m = new SimpleMaterialPBR();

		m->sphereColor = { fd(ren), fd(ren), fd(ren) };
		m->roughness = fd(ren);
		m->ambientOcclusion = fd(ren);
		m->metallic = fd(ren);
		stockMaterials.push_back(m);
	}

	std::uniform_int_distribution<int> d(0, MAX_MATERIAL_COUNT - 1);
	for (int i = 0; i < MAX_OBJECT_COUNT * MAX_MATERIAL_COUNT; ++i)
	{
		float x = (float)(i % 100) * 2.f - 50.f;
		float y = (float)(i / 100) * 2.f - 50.f;

		SceneObject*		object = new SceneObject({ 0, 0, 0 }, { 0, 0, 0 }, 1);
		object->Move({ x, y, -50.f });
		Mesh3D* sphereMesh = MESH::BuildSphere();
		

		const auto& model = object->GetModelToWorld();
		const mat3	 normalMat = glm::transpose(glm::inverse(model));
		


		for (const auto& v : sphereMesh->GetPoints())
		{
			vec4<float> temp = model * vec4<float>(v, 1.0f);
			vec3<float> bakedPosition = vec3<float>(temp.x, temp.y, temp.z);
			vec3<float> normal;
			vec2<float> texCoord;
		}
		for (int i = 0; i < sphereMesh->GetIndicesCount(); ++i)
		{
			//indices.push_back(baseIndex + i);
		}
		//sphereMesh->GetPoints();
		//VerticesDescription layout{
		//	VerticesDescription::Type::Position,
		//	VerticesDescription::Type::Normal,
		//	VerticesDescription::Type::TextureCoordinate
		//};

		//VertexObject* vertexObjects = new VertexObject(sphereMesh, layout);
		//sphere->AddMesh(sphereMesh);
		//sphere->vertexObjects.push_back(vertexObjects);


		//SceneObject* object = SceneObject::CreateSphere({ 0, 0, 0 });
		//object->SetObjectType(ObjectType::NonTextured);
		//object->Move({ x, y, -50.f });
		//objects.push_back(object);

		//SimpleMaterialPBR* m = stockMaterials[d(ren)];
		//RenderCommand cmd{ pbrShader, object->GetVO(), object->GetModelToWorld(), m };
		//rq.Push(cmd);
	}
}

void StressTestStage::Update(float dt)
{
	GlobalUniformManager::GetInstance().Update();

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

	Shader* pbrShader = ShaderManager::GetShader(ShaderDefinition::PBR);
	pbrShader->Use();
	envMap.BindIBLTexture(pbrShader);

	//rq.Sort();
	//rq.Draw();

	// TODO: clear should be go to shutdown or stage clear function
	//rq.Clear();


	//vec3<float> sphereColor{ 1.f, 0.f, 0.f };
	//float roughness = 0.1f;
	//float ambientOcclusion = 0.1f;
	//float metallic = 0.8f;

	//pbrShader->SendUniformVariable("albedo", sphereColor);
	//pbrShader->SendUniformVariable("roughness", roughness);
	//pbrShader->SendUniformVariable("ao", ambientOcclusion);
	//pbrShader->SendUniformVariable("metallic", metallic);

	//for (SceneObject* object : objects)
	//{
	//	pbrShader->SendUniformVariable("model", object->GetModelToWorld());
	//	object->Draw();
	//}

	const mat4<float>& View = camera.BuildViewMatrix();
	const mat4<float>& Projection = view.BuildProjectionMatrix();
	envMap.Render(Matrix4::CutOffTranslation(View), Projection);
}

void StressTestStage::DrawGUI()
{
	//ImGui_ImplOpenGL3_NewFrame();
	//ImGui_ImplGlfw_NewFrame();

	////ImGui::Render();
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
