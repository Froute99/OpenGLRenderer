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

static unsigned int globalBindingPoint = 1;

enum BindingSlot
{
	SLOT_MATRIX = 0,		// SLOT_GLOBAL
	SLOT_OBJECT = 1,
	SLOT_MATERIAL = 2,
	MAX_SLOTS
};

// Generalized Uniform Data
struct GUD
{
	//mat4<float> ndcMatrix;
	mat4<float> viewMatrix;
	mat4<float> projectionMatrix;
	//vec3<float> lightPosition;
	//vec3<float> lightColor;
	//float		lightIntensity;
	//vec3<float> cameraPosition;
	//uniform vec3 lightPosition;
	//uniform vec3 lightColors;
	//uniform float lightIntensity;
	//uniform vec3 camPos;
};

class GUBO			// Later, this class will be substitute UBO, and change the name to UBO
{
public:
	GUBO();
	void Bind(unsigned int shaderHandle);
	void Update(GUD newData);			// Seralization

private:
	unsigned int handle;

};

class GlobalUniformManager		// Singleton, Observer, DirtyFlag
{
	using Updator = std::function<void(GUD&)>;
public:
	static GlobalUniformManager& GetInstance()
	{
		static GlobalUniformManager instance;
		return instance;
	}

	GlobalUniformManager() {}

	void Init(unsigned int shaderHandle) { ubo.Bind(shaderHandle); }

	void RegisterUpdator(Updator updator)
	{
		updators.push_back(updator);
	}

	void Update()
	{
		//if (!isDirty)
		//	return;

		for (auto& updator : updators)
		{
			updator(newGUD);
		}
		// GUBO update with newGUD
		ubo.Update(newGUD);

		isDirty = false;
	}

	void SetDirty() { isDirty = true; }

//private:
	GUD									   newGUD;
	std::vector<Updator> updators;
	GUBO								   ubo;

	bool isDirty = true;		// Dirty Flag

	GlobalUniformManager(const GlobalUniformManager&) = delete;
	GlobalUniformManager& operator=(const GlobalUniformManager&) = delete;
	GlobalUniformManager(GlobalUniformManager&&) = delete;
	GlobalUniformManager& operator=(GlobalUniformManager&&) = delete;
};

class UBO
{
public:
	UBO(int size);
	void BindTo(const unsigned int shaderHandle, const char* blockName) noexcept;
	void WriteData(unsigned int offset, unsigned int size, const void* data);
	
	// For debug
	unsigned int GetHandle() const noexcept { return handle; }

private:
	unsigned int handle;
	unsigned int bindingPoint = 0;
	//unsigned int offset = 0;

};
