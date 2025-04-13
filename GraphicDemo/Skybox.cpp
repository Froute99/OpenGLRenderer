#include "Skybox.h"
#include <glew.h>

float skyboxVertices[] = {
	// positions
	-10.0f, 10.0f, -10.0f,
	-10.0f, -10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,
	10.0f, 10.0f, -10.0f,
	-10.0f, 10.0f, -10.0f,

	-10.0f, -10.0f, 10.0f,
	-10.0f, -10.0f, -10.0f,
	-10.0f, 10.0f, -10.0f,
	-10.0f, 10.0f, -10.0f,
	-10.0f, 10.0f, 10.0f,
	-10.0f, -10.0f, 10.0f,

	10.0f, -10.0f, -10.0f,
	10.0f, -10.0f, 10.0f,
	10.0f, 10.0f, 10.0f,
	10.0f, 10.0f, 10.0f,
	10.0f, 10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,

	-10.0f, -10.0f, 10.0f,
	-10.0f, 10.0f, 10.0f,
	10.0f, 10.0f, 10.0f,
	10.0f, 10.0f, 10.0f,
	10.0f, -10.0f, 10.0f,
	-10.0f, -10.0f, 10.0f,

	-10.0f, 10.0f, -10.0f,
	10.0f, 10.0f, -10.0f,
	10.0f, 10.0f, 10.0f,
	10.0f, 10.0f, 10.0f,
	-10.0f, 10.0f, 10.0f,
	-10.0f, 10.0f, -10.0f,

	-10.0f, -10.0f, -10.0f,
	-10.0f, -10.0f, 10.0f,
	10.0f, -10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,
	-10.0f, -10.0f, 10.0f,
	10.0f, -10.0f, 10.0f
};

#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>

Skybox::Skybox()
{
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// ==================================
	// since using Y up coordinate system
	//	GL_TEXTURE_CUBE_MAP_POSITIVE_X Right
	//	GL_TEXTURE_CUBE_MAP_NEGATIVE_X Left
	//	GL_TEXTURE_CUBE_MAP_POSITIVE_Y Top
	//	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y Bottom
	//	GL_TEXTURE_CUBE_MAP_POSITIVE_Z Front
	//	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z Back
	// ==================================

	//               OpenGL                               Mine
	//                +Y    -Z			                   +Y    +Z
	//                 |    /                               |    /
	//                 |   /                                |   /
	//                 |  /                                 |  /
	//                 | /                                  | /
	//                 |/                                   |/
	// -X -------------|------------- +X    -X -------------|------------- +X
	//                /|                                   /|
	//               / |                                  / |
	//              /  |                                 /  |
	//             /   |                                /   |
	//            /    |                               /    |
	//          +Z    -Y                              -Z   -Y

	cubemapPaths.push_back("../assets/skybox/right.jpg");
	cubemapPaths.push_back("../assets/skybox/left.jpg");
	cubemapPaths.push_back("../assets/skybox/bottom.jpg");
	cubemapPaths.push_back("../assets/skybox/top.jpg");
	cubemapPaths.push_back("../assets/skybox/front.jpg");
	cubemapPaths.push_back("../assets/skybox/back.jpg");

	glGenTextures(1, &cubemapTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureID);

	int w, h, nrChannels;
	for (unsigned int i = 0; i < cubemapPaths.size(); i++)
	{
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(cubemapPaths[i].c_str(), &w, &h, &nrChannels, 0);
		if (data)
		{
			std::cout << "succeed to load " << cubemapPaths[i] << std::endl;
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
				w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::Draw()
{
	glDepthFunc(GL_LEQUAL); // change depth function so depth test passes when values are equal to depth buffer's content
	//Shader::UseShader(skyboxShader);
	//const mat4<float>& LightCubeModel = cube->GetModelToWorld();
	//const mat4<float>& skyboxView = Matrix4::CutOffTranslation(View); // remove translation from origin view matrix
	//glUniformMatrix4fv(uniformLightCubeModel, 1, GL_FALSE, &LightCubeModel.elements[0][0]);
	//glUniformMatrix4fv(uniformLightCubeView, 1, GL_FALSE, &skyboxView.elements[0][0]);
	//glUniformMatrix4fv(uniformLightCubeProjection, 1, GL_FALSE, &Projection.elements[0][0]);

	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}
