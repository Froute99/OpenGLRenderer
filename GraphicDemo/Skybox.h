#pragma once
#include "Object.h"
#include <vector>

class Skybox : public Object
{
public:
	Skybox();

	virtual void Draw() override;

private:
	std::vector<std::string> cubemapPaths;
	unsigned int			 cubemapTextureID;
	unsigned int			 skyboxVAO;
	unsigned int			 skyboxVBO;

};
