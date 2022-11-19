/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Vec3UnitTest.cpp
 *	
 *	Vec3 Unit Test
 *	
 *	Fall 2019
 *	Dec.06 2019
 */

#include <limits>
#include <catch.hpp>
#include <vec3.hpp>

namespace
{
	constexpr auto e = std::numeric_limits<float>::epsilon();
}

TEST_CASE("Vec3 Constructors")
{
	SECTION("Default")
	{
		constexpr vec3<float> v;
		REQUIRE((v.x == 0.0f && v.y == 0.0f));
	}
	SECTION("Three Parameter")
	{
		constexpr vec3<float> v{ 1.0f, 2.0f, 3.0f };
		REQUIRE((v.x == 1.0f && v.y == 2.0f && v.z == 3.0f));
	}
	SECTION("Repeated Value")
	{
		constexpr vec3<float> v{ 1.0f };
		REQUIRE((v.x == 1.0f && v.y == 1.0f && v.z == 1.0f));
	}
}

TEST_CASE("Vec3 Arithmetic operators")
{
	constexpr vec3<float> v1{ 1.0f, 2.0f, 3.0f };
	constexpr vec3<float> v2{ 4.0f, 5.0f, 6.0f };
	vec3<float> result;
	SECTION("+")
	{
		result = v1 + v2;
		constexpr vec3<float> answer{ 5.0f, 7.0f, 9.0f };
		REQUIRE((abs(result.x - answer.x) <= e && abs(result.y - answer.y) <= e && abs(result.z - answer.z) <= e));
	}
	SECTION("-")
	{
		result = v1 - v2;
		constexpr vec3<float> answer{ -3.0f, -3.0f, -3.0f };
		REQUIRE((abs(result.x - answer.x) <= e && abs(result.y - answer.y) <= e && abs(result.z - answer.z) <= e));
	}
	SECTION("*")
	{
		result = v1 * 5.0f;
		constexpr vec3<float> answer{ 5.0f, 10.0f, 15.0f };
		REQUIRE((abs(result.x - answer.x) <= e && abs(result.y - answer.y) <= e && abs(result.z - answer.z) <= e));
	}
	SECTION("/")
	{
		result = v1 / 2.0f;
		constexpr vec3<float> answer{ 0.5f, 1.0f, 1.5f };
		REQUIRE((abs(result.x - answer.x) <= e && abs(result.y - answer.y) <= e && abs(result.z - answer.z) <= e));
	}
}

TEST_CASE("Vec3 Arithmetic with Assign Operators")
{
	vec3<float> v{ 2.0f, 3.0f, 5.0f };
	vec3<float> result{ 0.0f };
	SECTION("+=")
	{
		result += v;
		constexpr vec3<float> answer{ 2.0f, 3.0f, 5.0f };
		REQUIRE((abs(result.x - answer.x) <= e && abs(result.y - answer.y) <= e && abs(result.z - answer.z) <= e));
	}
	SECTION("-=")
	{
		result -= v;
		constexpr vec3<float> answer{ -2.0f, -3.0f, -5.0f };
		REQUIRE((abs(result.x - answer.x) <= e && abs(result.y - answer.y) <= e && abs(result.z - answer.z) <= e));
	}
	SECTION("*=")
	{
		v *= 2.0f;
		constexpr vec3<float> answer{ 4.0f, 6.0f, 10.0f };
		REQUIRE((abs(v.x - answer.x) <= e && abs(v.y - answer.y) <= e && abs(v.z - answer.z) <= e));
	}
	SECTION("/=")
	{
		v /= 2.0f;
		constexpr vec3<float> answer{ 1.0f, 1.5f, 2.5f };
		REQUIRE((abs(v.x - answer.x) <= e && abs(v.y - answer.y) <= e && abs(v.z - answer.z) <= e));
	}
}

TEST_CASE("Vec3 Logical Operators")
{
	constexpr vec3<float> v{ 2.0f, 3.0f, 1.0f };
	SECTION("-v")
	{
		constexpr vec3<float> result = -v;
		constexpr vec3<float> answer{ -2.0f, -3.0f, -1.0f };
		REQUIRE((abs(result.x - answer.x) <= e && abs(result.y - answer.y) <= e));
	}
	SECTION("==")
	{
		bool isSame = (v == vec3<float>{ 2.0f, 3.0f, 1.0f });
		REQUIRE(isSame == true);
	}
	SECTION("!=")
	{
		bool isNotSame = (v != vec3<float>{ 2.0f, 1.0f, 3.0f });
		REQUIRE(isNotSame == true);
	}
}

TEST_CASE("Vec3 Other Functions")
{
	constexpr vec3<float> v1{ 1.0f, 2.0f, 3.0f };
	constexpr vec3<float> v2{ 6.0f, 5.0f, 4.0f };
	SECTION("Dot Product")
	{
		const float result = dot_product(v1, v2);
		REQUIRE(abs(result - 28.0f) <= e);
	}
	SECTION("Cross Product")
	{
		vec3<float> result = cross_product(v1, v2);
		vec3<float> answer{ -7.0f, 14.0f, -7.0f };
		REQUIRE((abs(result.x - answer.x) <= e && abs(result.y - answer.y) <= e && abs(result.z - answer.z) <= e));
	}
	SECTION("Magnitude Squared")
	{
		const float result = magnitude_squared(v1);
		REQUIRE(abs(result - 14.0f) <= e);
	}
	SECTION("Magnitude")
	{
		const float result = magnitude(v1);
		REQUIRE(abs(result - 3.741657386773941f) <= e);
	}
	SECTION("Distance Between")
	{
		const float result = distance_between(v1, v2);
		REQUIRE(abs(result - 5.916079783099616f) <= e);
	}
	SECTION("Angle Between")
	{
		const float result = angle_between(v1, v2);
		REQUIRE(abs(result - 0.549467244757627f) <= e);
	}
	SECTION("Normalize")
	{
		const vec3<float> result = normalize(v1);
		const vec3<float> answer = { 0.2672612419124244f, 0.534522483824848f, 0.8017837257372732f };
		REQUIRE((abs(result.x - answer.x) <= e && abs(result.y - answer.y) <= e && abs(result.z - answer.z) <= e));
	}
}