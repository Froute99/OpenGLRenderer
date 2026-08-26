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
#include <random>

#include "Material.h"

#include <Graphics/Draw.h>

#include "Log.h"

std::ostream& operator<<(std::ostream& os, const mat4<float>& m)
{
	for (int i = 0; i < 4; ++i)
	{
		os << m[i][0] << ", "
		   << m[i][1] << ", "
		   << m[i][2] << ", "
		   << m[i][3] << std::endl;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const vec4<float>& v)
{
	os << v[0] << ", "
	   << v[1] << ", "
	   << v[2] << ", "
	   << v[3] << std::endl;
	return os;
}

StressTestStage::~StressTestStage() noexcept
{
	// TODO: clear should be go to shutdown or stage clear function
	//rq.Clear();
}

void StressTestStage::Initialize()
{
	//Shader* pbrShader = ShaderManager::GetShader(ShaderDefinition::PBR);
	Shader* pbrShader = ShaderManager::GetShader(ShaderDefinition::Test);
	if (!envMap.CanLoad("../assets/newport_loft.hdr", view.BuildProjectionMatrix()))
	{
		std::cout << "Failed to load env map\n";
	}

	GlobalUniformManager& uniformManager = GlobalUniformManager::GetInstance();
	uniformManager.Init(pbrShader->GetHandleToShader());
	uniformManager.SetLightPosition({ 3.6f, 0.55f, -0.4f });
	uniformManager.SetLightColor({ 1.f });
	uniformManager.SetLightIntensity(200.f);

	const int MAX_OBJECT_COUNT = 10000;
	const int MAX_MATERIAL_COUNT = 1;

	std::random_device					  rd;
	//std::mt19937						  ren(rd());		// random engine
	std::mt19937						  ren(0);		// random engine
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

	unsigned int baseIndex = 0;
	std::vector<vec3<float>> positions;
	std::vector<vec3<float>> normals;
	std::vector<vec2<float>> texCoords;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	std::uniform_int_distribution<int> d(0, MAX_MATERIAL_COUNT - 1);
	//material = stockMaterials[d(ren)];
	material = stockMaterials[0];
	for (int i = 0; i < MAX_OBJECT_COUNT * MAX_MATERIAL_COUNT; ++i)
	{
		float x = (float)(i % 100) * 2.f - 100.f;
		float y = (float)(i / 100) * 2.f - 100.f;
		
		/*** Static Batching **************/
		SceneObject*		object = new SceneObject({ 0, 0, 0 }, { 0, 0, 0 }, 1);
		object->SetObjectType(ObjectType::NonTextured);
		object->Move({ x, y, -100.f });
		Mesh3D* cubeMesh = MESH::BuildCube(1.f);
		object->AddMesh(cubeMesh);

		mat4<float> model = object->GetModelToWorld();
		mat4<float> normalMatrix = model.Inverse().Transpose();
		
		baseIndex = positions.size();
		for (int j = 0; j < cubeMesh->GetPointsCount(); ++j)
		{
			vec4<float> temp = model * vec4<float>(cubeMesh->GetPoint(j), 1.0f);

			vec3<float> bakedPosition = vec3<float>(temp.x, temp.y, temp.z);
			vec3<float> normal = normalMatrix.ToMat3() * cubeMesh->GetNormal(j);
			vec2<float> texCoord = cubeMesh->GetTextureCoordinate(j);

			positions.push_back(bakedPosition);
			normals.push_back(normal);
			texCoords.push_back(texCoord);
		}
		for (int h = 0; h < cubeMesh->GetIndicesCount(); ++h)
		{
			indices.push_back(baseIndex + cubeMesh->GetIndex(h));
		}

		/*** Render Queue **************/
		//SceneObject* object = SceneObject::CreateCube({ 0, 0, 0 }, { 0, 0, 0 }, 1.f);
		//object->SetObjectType(ObjectType::NonTextured);
		//object->Move({ x, y, -100.f });
		//objects.push_back(object);

		//RenderCommand cmd{ pbrShader, object->GetVO(), object->GetModelToWorld(), material };
		//rq.Push(cmd);
	}

	/*** Static Batching **************/
	unsigned int posSize = positions.size() * sizeof(vec3<float>);
	unsigned int normalSize = normals.size() * sizeof(vec3<float>);
	unsigned int tcSize = texCoords.size() * sizeof(vec2<float>);
	unsigned int totalSize = posSize + normalSize + tcSize;

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, posSize, positions.data());
	glBufferSubData(GL_ARRAY_BUFFER, posSize, normalSize, normals.data());
	glBufferSubData(GL_ARRAY_BUFFER, posSize + normalSize, tcSize, texCoords.data());

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)posSize);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(posSize + normalSize));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void StressTestStage::Update(float dt)
{
	GlobalUniformManager::GetInstance().Update();

	frameTimeGraph.Push(dt * 1000.f);
	Log::Add("Frame time average of recent 60 frames: " + std::to_string(frameTimeGraph.GetAvgFrameTime()) + " ms");
	gpuTimer.Begin();		// Start measuring GPU time

	/*** Static Batching **************/
	Shader* pbrShader = ShaderManager::GetShader(ShaderDefinition::Test);
	pbrShader->Use();
	material->ApplyToShader(pbrShader);
	Draw::DrawElementsWithCount(vao, GL_TRIANGLES, indices.size());


	/*** Render Queue **************/
	//Shader* pbrShader = ShaderManager::GetShader(ShaderDefinition::PBR);
	//rq.Sort();
	//rq.Draw();


	// IBL environment map
	const mat4<float>& View = camera.BuildViewMatrix();
	const mat4<float>& Projection = view.BuildProjectionMatrix();
	envMap.BindIBLTexture(pbrShader);
	envMap.Render(View.CutOffTranslation(), Projection);

	gpuTimer.End();		// End of measuring GPU time

	frameTimeGraph.cpuMs = dt * 1000.f;
	frameTimeGraph.gpuMs = gpuTimer.GetElapsedTime();

	Log::Add("Draw calls: " + std::to_string(Draw::GetDrawCount()));
	Log::Flush();
}

void StressTestStage::DrawGUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();
	frameTimeGraph.Render();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
