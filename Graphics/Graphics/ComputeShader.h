#pragma once
#include <string>
#include <filesystem>
#include <Math/vec2.hpp>
#include <Graphics/Texture.h>

class ComputeShader
{
public:
	ComputeShader() noexcept = default;
	
	void Initialize(unsigned int x, unsigned int y);

	bool LoadShader(const std::filesystem::path& path) noexcept;
	void CreateTexture(unsigned int x, unsigned int y);

	void Use();
	void BindTexture();
	void Dispatch(unsigned int x, unsigned int y, unsigned int z);
	void Wait();

	void Run();

	unsigned int GetHandle() noexcept { return handle; }
	void SetWorkGroupSize(const vec2<unsigned int> size) noexcept { workSize = size; }

private:
	unsigned int handle;

	unsigned int textureHandle;

	vec2<unsigned int> workSize;

};

