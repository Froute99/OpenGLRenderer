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
#include <Math/Angle.hpp>

Camera::Camera(vec3<float> newPos) noexcept
{
	eye = newPos;
}

// eye space, camera space, view space
mat4<float> Camera::BuildViewMatrix() const noexcept
{
	vec3<float> f = Vector3::normalize(target - eye);
	vec3<float> r = Vector3::normalize(Vector3::cross_product(up, f));
	vec3<float> u = Vector3::cross_product(f, r);

	mat4<float> viewMatrix = Matrix4::build_identity<float>();

	viewMatrix.elements[0][0] = r.x;
	viewMatrix.elements[1][0] = r.y;
	viewMatrix.elements[2][0] = r.z;
	viewMatrix.elements[0][1] = u.x;
	viewMatrix.elements[1][1] = u.y;
	viewMatrix.elements[2][1] = u.z;
	viewMatrix.elements[0][2] = f.x;
	viewMatrix.elements[1][2] = f.y;
	viewMatrix.elements[2][2] = f.z;
	viewMatrix.elements[3][0] = -Vector3::dot_product(r, eye);
	viewMatrix.elements[3][1] = -Vector3::dot_product(u, eye);
	viewMatrix.elements[3][2] = -Vector3::dot_product(f, eye);
	viewMatrix.elements[3][3] = 1.0f;

	return viewMatrix;
}
