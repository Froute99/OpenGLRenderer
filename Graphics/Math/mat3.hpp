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
#include <complex>	// cos, sin
#include <cassert>	// assert
#include <vec3.hpp>

template <typename T>
class mat3
{
public:
	constexpr mat3() noexcept
	{
		column[0].x = 0;
		column[0].y = 0;
		column[0].z = 0;

		column[1].x = 0;
		column[1].y = 0;
		column[1].z = 0;

		column[2].x = 0;
		column[2].y = 0;
		column[2].z = 0;
	}
	constexpr mat3(vec3<T> column0, vec3<T> column1, vec3<T> column2) noexcept
	{
		column[0] = column0;
		column[1] = column1;
		column[2] = column2;
	}
	constexpr mat3(T column0_row0, T column0_row1, T column0_row2,
		T column1_row0, T column1_row1, T column1_row2,
		T column2_row0, T column2_row1, T column2_row2)	noexcept
	{
		column[0].x = column0_row0;
		column[0].y = column0_row1;
		column[0].z = column0_row2;

		column[1].x = column1_row0;
		column[1].y = column1_row1;
		column[1].z = column1_row2;

		column[2].x = column2_row0;
		column[2].y = column2_row1;
		column[2].z = column2_row2;
	}
	constexpr mat3(T repeated_value) noexcept
	{
		column[0].x = repeated_value;
		column[0].y = repeated_value;
		column[0].z = repeated_value;

		column[1].x = repeated_value;
		column[1].y = repeated_value;
		column[1].z = repeated_value;

		column[2].x = repeated_value;
		column[2].y = repeated_value;
		column[2].z = repeated_value;
	}

	union
	{
		T elements[3][3];
		vec3<T> column[3];
	};
	constexpr T operator() (int col, int row) const noexcept
	{
		assert(0 <= col && col <= 2);
		assert(0 <= row && row <= 2);
		return elements[col][row];
	}

	constexpr T& operator() (int col, int row) noexcept
	{
		assert(0 <= col && col <= 2);
		assert(0 <= row && row <= 2);
		return elements[col][row];
	}
};

template <typename T>
mat3<T> operator*(const mat3<T>& m1, const mat3<T>& m2) noexcept
{
	mat3<T> m;
	for (int i = 0; i <= 2; ++i)
	{
		for (int j = 0; j <= 2; ++j)
		{
			m.elements[i][j] =   m1.elements[0][j] * m2.elements[i][0] 
				+ m1.elements[1][j] * m2.elements[i][1] 
				+ m1.elements[2][j] * m2.elements[i][2];
		}
	}
	return m;
}

template <typename T>
void operator*=(mat3<T>& m1, const mat3<T>& m2) noexcept
{
	m1 = m1 * m2;
}

namespace Matrix3
{
	template <typename T>
	constexpr mat3<T> build_translation(T translate_x, T translate_y) noexcept
	{
		return mat3<T>{
			1, 0, 0,
				0, 1, 0,
				translate_x, translate_y, 1 };
	}

	template <typename T>
	constexpr mat3<T> build_translation(T translate) noexcept
	{
		return mat3<T>{
			1, 0, 0,
				0, 1, 0,
				translate, translate, 1};
	}

	template <typename T>
	mat3<T> build_rotation(T angle_in_radians) noexcept
	{
		return mat3<T>{
			cos(angle_in_radians), -sin(angle_in_radians), 0,
				sin(angle_in_radians), cos(angle_in_radians), 0,
				0, 0, 1 };
	}

	template <typename T>
	constexpr mat3<T> build_scaling(T scale) noexcept
	{
		return mat3<T>{
			scale, 0, 0,
				0, scale, 0,
				0, 0, 1 };
	}

	template <typename T>
	constexpr mat3<T> build_scaling(T scale_x, T scale_y) noexcept
	{
		return mat3<T>{
			scale_x, 0, 0,
				0, scale_y, 0,
				0, 0, 1 };
	}

	template <typename T>
	constexpr mat3<T> transpose(const mat3<T>& m) noexcept
	{
		return mat3<T>{
			m.column[0].x, m.column[1].x, m.column[2].x,
				m.column[0].y, m.column[1].y, m.column[2].y,
				m.column[0].z, m.column[1].z, m.column[2].z };
	}
}
