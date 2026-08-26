/*
 *	Author: JeongHak Kim
 *	File_name: UBO.h
 *
 *	Uniform Buffer Object
 *
 *	2025. 07. 14
 */

#pragma once
#include "Shader.h"
#include <vector>
#include <functional>

#include <iostream>

static unsigned int globalBindingPoint = 1;

enum BindingSlot
{
	SLOT_MATRIX = 0,		// SLOT_GLOBAL
	SLOT_MATERIAL = 1,
	MAX_SLOTS
};

#pragma warning(push)
#pragma warning(disable : 4324)
// Generalized Uniform Data
struct GlobalUniformData
{
	//mat4<float> ndcMatrix;
	mat4<float> viewMatrix;				// 0
	mat4<float> projectionMatrix;		// 64
	vec3<float> lightPosition;			// 128
	alignas(16) vec3<float> lightColor; // 144
	float		lightIntensity = 1.f;	// 160
	vec3<float> cameraPosition;			// 176
};
#pragma warning(pop)

class GlobalUniformBuffer			// Later, this class will be substitute UBO, and change the name to UBO
{
public:
	GlobalUniformBuffer();
	void Bind(unsigned int shaderHandle);
	void Update(GlobalUniformData newData);			// Seralization

private:
	unsigned int handle;

};

// Split to separated header
class GlobalUniformManager		// Singleton, Observer, DirtyFlag
{
	using Updator = std::function<void(GlobalUniformData&)>;
public:
	static GlobalUniformManager& GetInstance()
	{
		static GlobalUniformManager instance;
		return instance;
	}

	GlobalUniformManager() {}

	void Init(unsigned int shaderHandle) { buffer.Bind(shaderHandle); }

	void RegisterUpdator(Updator updator)
	{
		updators.push_back(updator);
	}

	void Update()
	{
		if (!isDirty)
			return;
		for (auto& updator : updators)
		{
			updator(data);
		}
		// GlobalUniformBuffer update with data
		buffer.Update(data);

		isDirty = false;
	}

	void SetDirty() { isDirty = true; }

	// Temporal
	void SetLightPosition(const vec3<float>& value) { data.lightPosition = value; }
	void SetLightColor(const vec3<float>& value) { data.lightColor = value; }
	void SetLightIntensity(const float value) { data.lightIntensity = value; }

private:
	GlobalUniformData	 data;
	std::vector<Updator> updators;
	GlobalUniformBuffer				 buffer;

	bool isDirty = true;		// Dirty Flag

	GlobalUniformManager(const GlobalUniformManager&) = delete;
	GlobalUniformManager& operator=(const GlobalUniformManager&) = delete;
	GlobalUniformManager(GlobalUniformManager&&) = delete;
	GlobalUniformManager& operator=(GlobalUniformManager&&) = delete;

};
