/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Animation.cpp
 *
 *	Animation class
 *
 *	Fall 2019
 *	Nov.01 2019
 */

#include "Animation.h"

void Animation::Initialize(SpriteSheet new_sheet, const Shader& new_shader)
{
	spriteSheet = new_sheet;
	shader = new_shader;
}

void Animation::Animate(float dt)
{
	Shader::UseShader(shader);
	shader.SendUniformVariable("frameX", spriteSheet.frameX);
	shader.SendUniformVariable("frameY", spriteSheet.frameY);
	shader.SendUniformVariable("frameIndex", frameIndex);

	frameIndex = int(baseTime) % (spriteSheet.frameX * spriteSheet.frameY);
	baseTime += spriteSheet.animateSpeed * dt;
}

void Animation::ChangeAnimation(int new_count)
{
	spriteSheet.frameX = new_count;
}
