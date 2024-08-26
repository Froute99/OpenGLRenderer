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

mat3<float> Transform::GetModelToWorld() noexcept
{
	mat3<float> T = Matrix3::build_translation(translation.x, translation.y);
	mat3<float> R = Matrix3::build_rotation(rotation);
	mat3<float> S = Matrix3::build_scaling(scale.x, scale.y);
	mat3<float> M = T * R * S;

	if (parent != nullptr)
	{
		return parent->GetModelToWorld() * M;
	}
	return M;
}

mat3<float> Transform::GetWorldToModel() noexcept
{
	mat3<float> T_inverse = Matrix3::build_translation(-translation.x, -translation.y);
	mat3<float> R_inverse = Matrix3::build_rotation(-rotation);
	mat3<float> S_inverse = Matrix3::build_scaling(1 / scale.x, 1 / scale.y);
	mat3<float> M_inverse = S_inverse * R_inverse * T_inverse;

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
