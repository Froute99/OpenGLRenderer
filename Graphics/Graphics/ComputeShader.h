#pragma once
#include <string>
#include <filesystem>

class ComputeShader
{
public:
	ComputeShader() noexcept = default;
	bool LoadShader(const std::filesystem::path& path) noexcept;
	void Use() noexcept;
private:
	unsigned int handle;

};

