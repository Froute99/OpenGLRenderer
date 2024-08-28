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

Camera::Camera(vec3<float> camera_center, vec3<float> camera_up) noexcept
{
}

//void Camera::ResetUp(vec3<float> camera_up) noexcept
//{
//	up.x = camera_up.x;
//	up.y = camera_up.y;
//	right = { up.y, -up.x };
//}

//void Camera::MoveUp(float distance) noexcept
//{
//	center += Vector2::normalize(up) * distance;
//}

//void Camera::MoveRight(float distance) noexcept
//{
//	center += Vector2::normalize(right) * distance;
//}

//void Camera::Rotate(float angle_radians) noexcept
//{
//	up = rotate_by(angle_radians, up);
//	right = rotate_by(angle_radians, right);
//}

//mat4<float> Camera::CameraToWorld() const noexcept
//{
//	using namespace Vector2;
//	mat3<float> inverseTransformMatrix = {
//		up.y,  -right.y, (right.y * dot_product(-up, center) - dot_product(-right, center) * up.y),
//		-up.x, right.x,  (dot_product(-right, center) * up.x - right.x * dot_product(-up, center)),
//		0.0f,  0.0f, 1.0f };
//	return Matrix3::transpose(inverseTransformMatrix);
//}

//mat4<float> Camera::WorldToCamera() const noexcept
//{
//	using namespace Vector2;
//	mat3<float> transformMatrix = {
//		right.x, up.x, 0.0f,
//		right.y, up.y, 0.0f,
//		dot_product(right, center), dot_product(up, center),1.0f };
//	return transformMatrix;
//}

mat4<float> Camera::LookAt() const noexcept
{
	using namespace Vector3;
	vec3<float> const f = Vector3::normalize(center - target);
	vec3<float> const r = Vector3::cross_product(f, up);
	vec3<float> const u = Vector3::cross_product(r, f);

	//vec<3, T, Q> const f(normalize(center - eye));		// eye would be the target vector
	//vec<3, T, Q> const s(normalize(cross(f, up)));
	//vec<3, T, Q> const u(cross(s, f));

	mat4<float> result(0);
	result.elements[0][0] = r.x;
	result.elements[1][0] = r.y;
	result.elements[2][0] = r.z;
	result.elements[0][1] = u.x;
	result.elements[1][1] = u.y;
	result.elements[2][1] = u.z;
	result.elements[0][2] = f.x;
	result.elements[1][2] = f.y;
	result.elements[2][2] = f.z;
	result.elements[3][0] = -dot_product(r, target);
	result.elements[3][1] = -dot_product(u, target);
	result.elements[3][2] = -dot_product(f, target);
	result.elements[3][3] = 1.0f;

	//mat<4, 4, T, Q> Result(1);
	//Result[0][0] = s.x;
	//Result[1][0] = s.y;
	//Result[2][0] = s.z;
	//Result[0][1] = u.x;
	//Result[1][1] = u.y;
	//Result[2][1] = u.z;
	//Result[0][2] = -f.x;
	//Result[1][2] = -f.y;
	//Result[2][2] = -f.z;
	//Result[3][0] = -dot(s, eye);
	//Result[3][1] = -dot(u, eye);
	//Result[3][2] = dot(f, eye);
	return result;
}
