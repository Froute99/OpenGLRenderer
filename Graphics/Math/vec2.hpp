/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: vec2.hpp
 *
 *	Custom math library for vec2
 *
 *	Fall 2019
 *	Sep.20 2019
 */

#pragma once
#include <cassert>
#include <algorithm>

template <typename T>
class vec2
{
public:
	constexpr vec2() noexcept
		: x(0), y(0) {}

	constexpr vec2(T x, T y) noexcept
		: x(x), y(y) {}

	constexpr vec2(T repeated_value) noexcept
		: x(repeated_value), y(repeated_value) {}

	T x, y;
};

template <typename T>
constexpr void operator+=(vec2<T>& v, const vec2<T>& adding_vector) noexcept
{
	v.x += adding_vector.x;
	v.y += adding_vector.y;
}

template <typename T>
constexpr void operator-=(vec2<T>& v, const vec2<T>& subtracting_vector) noexcept
{
	v.x -= subtracting_vector.x;
	v.y -= subtracting_vector.y;
}

template <typename T>
constexpr void operator*=(vec2<T>& v, const T& scale) noexcept
{
	v.x *= scale;
	v.y *= scale;
}

template <typename T>
constexpr void operator/=(vec2<T>& v, const T& divisor) noexcept
{
	assert(divisor != 0.f);
	v.x /= divisor;
	v.y /= divisor;
}

template <typename T>
constexpr vec2<T> operator-(const vec2<T>& v) noexcept
{
	return vec2<T>{ -v.x, -v.y };
}

template <typename T>
constexpr vec2<T> operator+(const vec2<T>& v1, const vec2<T>& v2) noexcept
{
	return vec2<T>{ v1.x + v2.x, v1.y + v2.y };
}

template <typename T>
constexpr vec2<T> operator-(const vec2<T>& v1, const vec2<T>& v2) noexcept
{
	return vec2<T>{ v1.x - v2.x, v1.y - v2.y };
}

template <typename T>
constexpr vec2<T> operator*(const vec2<T>& v, const T& scale) noexcept
{
	return vec2<T>{ v.x* scale, v.y* scale };
}

template <typename T>
constexpr vec2<T> operator*(const T& scale, const vec2<T>& v) noexcept
{
	return v * scale;
}

template <typename T>
constexpr vec2<T> operator/(const vec2<T>& v, const T& divisor) noexcept
{
	assert(divisor != 0);
	return vec2<T>{ v.x / divisor, v.y / divisor };
}

template <typename T>
constexpr bool operator==(const vec2<T>& v1, const vec2<T>& v2) noexcept
{
	const vec2<T>& v = v1 - v2;
	if (abs(v.x) <= std::numeric_limits<T>::epsilon() && abs(v.y) <= std::numeric_limits<T>::epsilon())
	{
		return true;
	}
	return false;
}

template <typename T>
constexpr bool operator!=(const vec2<T>& v1, const vec2<T>& v2) noexcept
{
	if (v1 == v2)
	{
		return false;
	}
	return true;
}

namespace Vector2
{
	template <typename T>
	constexpr T dot_product(const vec2<T>& v1, const vec2<T>& v2) noexcept
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	template <typename T>
	constexpr T magnitude_squared(const vec2<T>& v) noexcept
	{
		return dot_product(v, v);
	}

	template <typename T>
	constexpr T magnitude(const vec2<T>& v) noexcept
	{
		return (T)sqrt(magnitude_squared(v));
	}

	template <typename T>
	constexpr T distance_between(const vec2<T>& v1, const vec2<T>& v2) noexcept
	{
		return sqrt(magnitude_squared(v1 - v2));
		//return magnitude(v1 - v2);
	}

	template <typename T>
	constexpr T angle_between(const vec2<T>& v1, const vec2<T>& v2) noexcept
	{
		assert(magnitude(v1) != 0 && magnitude(v2) != 0);
		return acos(dot_product(v1, v2) / magnitude(v1) * magnitude(v2));
	}

	template <typename T>
	constexpr vec2<T> rotate_by(const float& angle_in_radians, const vec2<T>& v) noexcept
	{
		float cos_value = cos(angle_in_radians);
		float sin_value = sin(angle_in_radians);
		return vec2<T>{ cos_value* v.x - sin_value * v.y, sin_value* v.x + cos_value * v.y };
	}

	template <typename T>
	constexpr vec2<T> normalize(const vec2<T>& v) noexcept
	{
		T mag = magnitude(v);
		assert(mag != 0);
		return v / mag;
	}
}
