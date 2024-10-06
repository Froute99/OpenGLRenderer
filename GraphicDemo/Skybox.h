#pragma once
#include "GameObject.h"

class Skybox : public GameObject
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
