/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Camera.cpp
 *
 *	Camera class
 *
 *	Fall 2019
 *	Oct.10 2019
 */

#include "Camera.h"
#include "Transform.h"
#include <Math/Angle.hpp>
#include <Graphics/UBO.h>

Camera::Camera(vec3<float> newPos) noexcept
{
	eye = newPos;
}

mat4<float> Camera::BuildViewMatrix() const noexcept
{
	return Matrix4::BuildLookAt(eye, eye + front, up);
}

void Camera::UniformRegistry(GUD& data)
{
	data.viewMatrix = BuildViewMatrix();
}

void Camera::SetFront(const vec3<float>& value) noexcept
{
	front = value;
	right = Vector3::normalize(Vector3::cross_product(front, {0, 1, 0}));
	up = Vector3::cross_product(right, front);
	GlobalUniformManager::GetInstance().SetDirty();
}

void Camera::MoveX(float distance) noexcept
{
	vec3<float> amount = distance * Vector3::normalize(right);
	eye += amount;
	GlobalUniformManager::GetInstance().SetDirty();
}

void Camera::MoveY(float distance) noexcept
{
	vec3<float> vertical = { 0.f, 1.f, 0.f };
	vec3<float> amount = distance * Vector3::normalize(vertical);
	eye += amount;
	GlobalUniformManager::GetInstance().SetDirty();
}

void Camera::MoveZ(float distance) noexcept
{
	vec3<float> amount = distance * front;
	eye += amount;
	GlobalUniformManager::GetInstance().SetDirty();
}
