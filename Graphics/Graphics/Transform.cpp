/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Transform.cpp
 *
 *	Transform
 *
 *	Fall 2019
 *	Oct.10 2019
 */

#include "Transform.h"

float Transform::CalculateWorldDepth() const noexcept
{
	if (parent != nullptr)
	{
		return depth + parent->CalculateWorldDepth();
	}
	return depth;
}

mat4<float> Transform::BuildModelMatrix() const noexcept
{
	const mat4<float>& T = Matrix4::build_translation(translation);
	const mat4<float>& R = Matrix4::build_rotation_euler(rotation);
	const mat4<float>& S = Matrix4::build_scaling(scale);

	mat4<float> modelToWorld = T * R * S;
	if (parent != nullptr)
	{
		modelToWorld = parent->BuildModelMatrix() * modelToWorld;
	}
	return modelToWorld;
}
