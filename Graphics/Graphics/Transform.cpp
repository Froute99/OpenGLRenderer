/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Transform.cpp
 *
 *	Transform
 *
 *	Fall 2019
 *	Oct.10 2019
 */

#include "Transform.hpp"

mat4<float> Transform::GetModelToWorld() noexcept
{
	using namespace Matrix4;
	mat4<float> T = build_translation(translation);
	mat4<float> R = build_rotation_euler(rotation);
	mat4<float> S = build_scaling(scale);
	mat4<float> M = T * R * S;

	if (parent != nullptr)
	{
		return parent->GetModelToWorld() * M;
	}
	return M;
}

mat4<float> Transform::GetWorldToModel() noexcept
{
	using namespace Matrix4;
	mat4<float> T_inverse = build_translation(-translation);
	mat4<float> R_inverse = build_rotation_euler(-rotation);
	mat4<float> S_inverse = build_scaling(1 / scale.x, 1 / scale.y, 1 / scale.z);
	mat4<float> M_inverse = S_inverse * R_inverse * T_inverse;

	if (parent != nullptr)
	{
		return M_inverse * parent->GetWorldToModel();
	}

	return M_inverse;
}

float Transform::CalculateWorldDepth() const noexcept
{
	if (parent != nullptr)
	{
		return depth + parent->CalculateWorldDepth();
	}
	return depth;
}
