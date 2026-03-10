/*
 *	Author: JeongHak Kim
 *	File_name: StressTestStage.h
 *
 *	Stress Test
 *
 *	2026. 03. 03
 */

#pragma once
#include "Demo.h"
#include <Graphics/Shader.h>
#include <Graphics/IBL.h>
#include <Graphics/UBO.h>
#include <vector>
#include "RenderQueue.h"			// optimization test

class SceneObject;
class StressTestStage : public Demo
{
public:
	explicit StressTestStage(OpenGLWindow& window)
		: Demo(window)
	{
		Initialize();
	}
	~StressTestStage() noexcept override {}

	void Initialize() override;
	void Update(float dt) override;

	void DrawGUI() override;

private:
	float frameTime = 0.f;
	int	  frameCount = 0;

	EnvironmentMap envMap{ GetScreenWidth(), GetScreenHeight() };
	std::vector<SceneObject*> objects;
	RenderQueue				  rq;

	std::vector<SimpleMaterialPBR*> stockMaterials;
};
