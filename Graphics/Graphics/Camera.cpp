/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Camera.cpp
 *
 *	Camera class
 *
 *	Fall 2019
 *	Oct.10 2019
 */

#include "Camera.hpp"
#include "Transform.hpp"

void Camera::ResetUp(vec2<float> camera_up) noexcept
{
	up.x = camera_up.x;
	up.y = camera_up.y;
	right = { up.y, -up.x };
}

void Camera::MoveUp(float distance) noexcept
{
	center += normalize(up) * distance;
}

void Camera::MoveRight(float distance) noexcept
{
	center += normalize(right) * distance;
}

void Camera::Rotate(float angle_radians) noexcept
{
	up = rotate_by(angle_radians, up);
	right = rotate_by(angle_radians, right);
}

mat3<float> Camera::CameraToWorld() const noexcept
{
	//return build_translation(center.x, center.y) * transpose(mat3<float>{
	//	right.x, up.x, 0.f,
	//	right.y, up.y, 0.f,
	//	0.f, 0.f, 1.f });
	mat3<float> inverseTransformMatrix = { up.y,  -right.y, (right.y * dot_product(-up, center) - dot_product(-right, center) * up.y),
		-up.x, right.x,  (dot_product(-right, center) * up.x - right.x * dot_product(-up, center)),
		0.0f,  0.0f,     1.0f };
	return transpose(inverseTransformMatrix);
}

mat3<float> Camera::WorldToCamera() const noexcept
{
	//return transpose(mat3<float>{
	//	right.x, right.y, 0.f,
	//	up.x, up.y, 0.f,
	//	0.f, 0.f, 1.f }) * build_translation(-center.x, -center.y);
	mat3<float> transformMatrix = {
		right.x, up.x, 0.0f,
		right.y, up.y, 0.0f,
		dot_product(right, center), dot_product(up, center),1.0f };
	return transformMatrix;
}
