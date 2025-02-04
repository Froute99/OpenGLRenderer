/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: mat3.hpp
 *
 *	Custom math library for mat3
 *
 *	Fall 2019
 *	Sep.20 2019
 */

#pragma once
#include <complex> // cos, sin
#include <cassert> // assert
#include "vec3.hpp"
#include "vec4.hpp"
#include "mat3.hpp"

template <typename T>
class mat4
{
public:
	constexpr mat4() noexcept
	{
		column[0].x = 0;
		column[0].y = 0;
		column[0].z = 0;
		column[0].w = 0;

		column[1].x = 0;
		column[1].y = 0;
		column[1].z = 0;
		column[2].w = 0;

		column[2].x = 0;
		column[2].y = 0;
		column[2].z = 0;
		column[2].w = 0;

		column[3].x = 0;
		column[3].y = 0;
		column[3].z = 0;
		column[3].w = 0;
	}
	constexpr mat4(vec4<T> column0, vec4<T> column1, vec4<T> column2, vec4<T> column3) noexcept
	{
		column[0] = column0;
		column[1] = column1;
		column[2] = column2;
		column[3] = column3;
	}
	constexpr mat4(
		T column0Row0, T column0Row1, T column0Row2, T column0Row3,
		T column1Row0, T column1Row1, T column1Row2, T column1Row3,
		T column2Row0, T column2Row1, T column2Row2, T column2Row3,
		T column3Row0, T column3Row1, T column3Row2, T column3Row3) noexcept
	{
		column[0].x = column0Row0;
		column[0].y = column0Row1;
		column[0].z = column0Row2;
		column[0].w = column0Row3;

		column[1].x = column1Row0;
		column[1].y = column1Row1;
		column[1].z = column1Row2;
		column[1].w = column1Row3;

		column[2].x = column2Row0;
		column[2].y = column2Row1;
		column[2].z = column2Row2;
		column[2].w = column2Row3;

		column[3].x = column3Row0;
		column[3].y = column3Row1;
		column[3].z = column3Row2;
		column[3].w = column3Row3;
	}
	constexpr mat4(T repeatedValue) noexcept
	{
		column[0].x = repeatedValue;
		column[0].y = repeatedValue;
		column[0].z = repeatedValue;
		column[0].w = repeatedValue;

		column[1].x = repeatedValue;
		column[1].y = repeatedValue;
		column[1].z = repeatedValue;
		column[1].w = repeatedValue;

		column[2].x = repeatedValue;
		column[2].y = repeatedValue;
		column[2].z = repeatedValue;
		column[2].w = repeatedValue;

		column[3].x = repeatedValue;
		column[3].y = repeatedValue;
		column[3].z = repeatedValue;
		column[3].w = repeatedValue;
	}

	union
	{
		T		elements[4][4];
		vec4<T> column[4];
	};
	constexpr T operator()(int col, int row) const noexcept
	{
		// assert(0 <= col && col <= 3, "mat4; exceed column index range 0 to 3");
		// assert(0 <= row && row <= 3, "mat4; exceed row index range 0 to 3");
		assert(0 <= col && col <= 3);
		assert(0 <= row && row <= 3);
		return elements[col][row];
	}

	constexpr T& operator()(int col, int row) noexcept
	{
		// assert(0 <= col && col <= 3, "mat4; exceed column index range 0 to 3");
		// assert(0 <= row && row <= 3, "mat4; exceed row index range 0 to 3");
		assert(0 <= col && col <= 3);
		assert(0 <= row && row <= 3);
		return elements[col][row];
	}
};

template <typename T>
mat4<T> operator*(const mat4<T>& m1, const mat4<T>& m2) noexcept
{
	mat4<T> m;
	for (int i = 0; i <= 3; ++i)
	{
		for (int j = 0; j <= 3; ++j)
		{
			m.elements[i][j] =
				  m1.elements[0][j] * m2.elements[i][0]
				+ m1.elements[1][j] * m2.elements[i][1]
				+ m1.elements[2][j] * m2.elements[i][2]
				+ m1.elements[3][j] * m2.elements[i][3];
		}
	}
	return m;
}

template <typename T>
void operator*=(mat4<T>& m1, const mat4<T>& m2) noexcept
{
	m1 = m1 * m2;
}

namespace Matrix4
{
	template <typename T>
	constexpr mat4<T> build_identity() noexcept
	{
		mat4<T> result(static_cast<T>(0));
		result.elements[0][0] = static_cast<T>(1);
		result.elements[1][1] = static_cast<T>(1);
		result.elements[2][2] = static_cast<T>(1);
		result.elements[3][3] = static_cast<T>(1);
		return result;
	}

	template <typename T>
	constexpr mat4<T> build_translation(T x, T y, T z) noexcept
	{
		return mat4<T>{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			x, y, z, 1
		};
	}

	template <typename T>
	constexpr mat4<T> build_translation(T t) noexcept
	{
		return mat4<T>{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			t, t, t, 1
		};
	}

	template <typename T>
	constexpr mat4<T> build_translation(const vec3<T>& t) noexcept
	{
		return mat4<T>{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			t.x, t.y, t.z, 1
		};
	}

	template <typename T>
	mat4<T> build_rotation_euler(T yaw, T pitch, T roll) noexcept
	{
		mat3<T> Z(
			cosf(yaw), -sinf(yaw), 0.f,
			sinf(yaw), cosf(yaw), 0.f,
			0.f, 0.f, 1.f);

		mat3<T> Y(
			cosf(pitch), 0.f, sinf(pitch),
			0.f, 1.f, 0.f,
			-sinf(pitch), 0.f, cosf(pitch));
		
		mat3<T> X(
			1.f, 0.f, 0.f,
			0.f, cosf(roll), -sinf(roll),
			0.f, sinf(roll), cosf(roll));


		mat3<T> tmp = Z * Y * X;
		mat4<T> result = {
			vec4<T>(tmp.column[0], 0),
			vec4<T>(tmp.column[1], 0),
			vec4<T>(tmp.column[2], 0),
			vec4<T>(0, 0, 0, 1)
		};
		return result;
	}

	template <typename T>
	mat4<T> build_rotation_euler(const vec3<T>& t) noexcept
	{
		mat3<T> Z(
			cosf(t.x), -sinf(t.x), 0.f,
			sinf(t.x), cosf(t.x), 0.f,
			0.f, 0.f, 1.f);

		mat3<T> Y(
			cosf(t.y), 0.f, sinf(t.y),
			0.f, 1.f, 0.f,
			-sinf(t.y), 0.f, cosf(t.y));

		mat3<T> X(
			1.f, 0.f, 0.f,
			0.f, cosf(t.z), -sinf(t.z),
			0.f, sinf(t.z), cosf(t.z));

		mat3<T> tmp = Z * Y * X;

		mat4<T> result = {
			vec4<T>(tmp.column[0], 0),
			vec4<T>(tmp.column[1], 0),
			vec4<T>(tmp.column[2], 0),
			vec4<T>(0, 0, 0, 1)
		};
		return result;
	}

	template <typename T>
	constexpr mat4<T> build_scaling(T s) noexcept
	{
		return mat4<T>{
			s, 0, 0, 0,
			0, s, 0, 0,
			0, 0, s, 0,
			0, 0, 0, 1
		};
	}

	template <typename T>
	constexpr mat4<T> build_scaling(T x, T y, T z) noexcept
	{
		return mat4<T>{
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1
		};
	}

	template <typename T>
	constexpr mat4<T> build_scaling(const vec3<T>& t) noexcept
	{
		return mat4<T>{
			t.x, 0, 0, 0,
			0, t.y, 0, 0,
			0, 0, t.z, 0,
			0, 0, 0, 1
		};
	}

	template <typename T>
	constexpr mat4<T> transpose(const mat4<T>& m) noexcept
	{
		return mat4<T>{
			m.column[0].x, m.column[1].x, m.column[2].x, m.column[3].x,
			m.column[0].y, m.column[1].y, m.column[2].y, m.column[3].y,
			m.column[0].z, m.column[1].z, m.column[2].z, m.column[3].z,
			m.column[0].w, m.column[1].w, m.column[2].w, m.column[3].w
		};
	}

	template <typename T>
	constexpr mat4<T> BuildLookAt(const vec3<T>& eye, const vec3<T>& lookAt, const vec3<T>& up)
	{
		vec3<float> f = Vector3::normalize(lookAt -eye);
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

	// we need 4 matrix build function
	// 1. General Projection Matrix						Skip this because I always use symmetric view frustum
	// 2. Infinite Far Plane Projection Matrix			Skip this because I always use symmetric view frustum
	// 3. General-Symmetric Projection Matrix
	// 4. Infinite-Symmetric Projection Matrix

	// General-symmetric projection
	template <typename T>
	constexpr mat4<T> GeneralProjectionMatrix(T fovYinRadians, T aspect, T zNear, T zFar)
	{
		T tangent = tan(fovYinRadians / static_cast<T>(2));

		mat4<T> result(0.0);
		result.elements[0][0] = static_cast<T>(1.0) / (aspect * tangent);
		result.elements[1][1] = static_cast<T>(1.0) / (tangent);
		result.elements[2][2] = zFar / (zNear - zFar);
		result.elements[2][3] = -static_cast<T>(1.0);
		result.elements[3][2] = -static_cast<T>(2.0) * (zFar * zNear) / (zFar - zNear);

		return result;
	}

	template <typename T>
	constexpr mat4<T> InfiniteProjectionMatrix(T fovYinRadians, T aspect, T zNear)
	{
		T tangent = tan(fovYinRadians / static_cast<T>(2));

		mat4<T> result(0.0);
		result.elements[0][0] = static_cast<T>(1.0) / (aspect * tangent);
		result.elements[1][1] = static_cast<T>(1.0) / (tangent);
		result.elements[2][2] = -static_cast<T>(1.0);
		result.elements[2][3] = -static_cast<T>(1.0);
		result.elements[3][2] = -static_cast<T>(2.0) * zNear;

		return result;
	}

	template <typename T>
	constexpr mat4<T> CutOffTranslation(const mat4<T>& m)
	{
		mat4<T> result = m;
		result.elements[0][3] = 0;
		result.elements[1][3] = 0;
		result.elements[2][3] = 0;

		result.elements[3][0] = 0;
		result.elements[3][1] = 0;
		result.elements[3][2] = 0;

		result.elements[3][3] = 1;

		return result;
	}
} // namespace Matrix4
