/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Mat3UnitTest.cpp
 *	
 *	Mat3 Unit Test
 *
 *	Fall 2019
 *	Dec.07 2019
 */

#include <catch.hpp>
#include <mat3.hpp>
#include <Angle.hpp>

namespace
{
	constexpr auto e = std::numeric_limits<float>::epsilon();
}

TEST_CASE("Mat3 Constructors")
{
	SECTION("Default")
	{
		const mat3<float> m;
		REQUIRE((m.elements[0][0] == 0.0f && m.elements[0][1] == 0.0f && m.elements[0][2] == 0.0f
			&& m.elements[1][0] == 0.0f && m.elements[1][1] == 0.0f && m.elements[1][2] == 0.0f
			&& m.elements[2][0] == 0.0f && m.elements[2][1] == 0.0f && m.elements[2][2] == 0.0f));
	}
	SECTION("Three Columns")
	{
		constexpr vec3<float> v1{ 1.0f, 2.0f, 3.0f };
		constexpr vec3<float> v2{ 4.0f, 5.0f, 6.0f };
		constexpr vec3<float> v3{ 7.0f, 8.0f, 9.0f };
		const mat3<float> m{ v1, v2, v3 };
		REQUIRE((m.column[0].x == 1.0f && m.column[0].y == 2.0f && m.column[0].z == 3.0f
			&& m.column[1].x == 4.0f && m.column[1].y == 5.0f && m.column[1].z == 6.0f
			&& m.column[2].x == 7.0f && m.column[2].y == 8.0f && m.column[2].z == 9.0f));
	}
	SECTION("Nine Numbers")
	{
		const mat3<float> m{ 1.0f, 2.0f, 3.0f,
							4.0f, 5.0f, 6.0f,
							7.0f, 8.0f, 9.0f };
		REQUIRE((m.elements[0][0] == 1.0f && m.elements[0][1] == 2.0f && m.elements[0][2] == 3.0f
			&& m.elements[1][0] == 4.0f && m.elements[1][1] == 5.0f && m.elements[1][2] == 6.0f
			&& m.elements[2][0] == 7.0f && m.elements[2][1] == 8.0f && m.elements[2][2] == 9.0f));
	}
	SECTION("Repated Value")
	{
		const mat3<float> m{ 1.0f };
		REQUIRE((m.column[0].x == 1.0f && m.column[0].y == 1.0f && m.column[0].z == 1.0f
			&& m.column[1].x == 1.0f && m.column[1].y == 1.0f && m.column[1].z == 1.0f
			&& m.column[2].x == 1.0f && m.column[2].y == 1.0f && m.column[2].z == 1.0f));
	}
	SECTION("Index Operator")
	{
		const mat3<float> m{ 1.0f, 2.0f, 3.0f,
							4.0f, 5.0f, 6.0f,
							7.0f, 8.0f, 9.0f };
		REQUIRE((abs(m(0, 0) - 1.0f) <= e && abs(m(0, 1) - 2.0f) <= e && abs(m(0, 2) - 3.0f) <= e
			&& abs(m(1, 0) - 4.0f) <= e && abs(m(1, 1) - 5.0f) <= e && abs(m(1, 2) - 6.0f) <= e
			&& abs(m(2, 0) - 7.0f) <= e && abs(m(2, 1) - 8.0f) <= e && abs(m(2, 2) - 9.0f) <= e));
	}
}

TEST_CASE("Mat3 Arithmetic Operators")
{
	const mat3<float> m1{ 1.0f, 2.0f, 3.0f,
						4.0f, 5.0f, 6.0f,
						7.0f, 8.0f, 9.0f };
	const mat3<float> m2{ 1.0f, 4.0f, 7.0f,
						2.0f, 5.0f, 8.0f,
						3.0f, 6.0f, 9.0f };
	SECTION("*")
	{
		const mat3<float> result = m1 * m2;
		REQUIRE((abs(result(0, 0) - 66.0f) <= e && abs(result(0, 1) - 78.0f) <= e && abs(result(0, 2) - 90.0f) <= e
			&& abs(result(1, 0) - 78.0f) <= e && abs(result(1, 1) - 93.0f) <= e && abs(result(1, 2) - 108.0f) <= e
			&& abs(result(2, 0) - 90.0f) <= e && abs(result(2, 1) - 108.0f) <= e && abs(result(2, 2) - 126.0f) <= e));
	}
	SECTION("*=")
	{
		mat3<float> result{ 1.0f };
		result *= m1;
		REQUIRE((abs(result(0, 0) - 6.0f) <= e && abs(result(0, 1) - 6.0f) <= e && abs(result(0, 2) - 6.0f) <= e
			&& abs(result(1, 0) - 15.0f) <= e && abs(result(1, 1) - 15.0f) <= e && abs(result(1, 2) - 15.0f) <= e
			&& abs(result(2, 0) - 24.0f) <= e && abs(result(2, 1) - 24.0f) <= e && abs(result(2, 2) - 24.0f) <= e));
	}
}

TEST_CASE("Building  Matrix")
{
	mat3<float> m;
	SECTION("Build Translation Matrix")
	{
		m = build_translation(2.0f, 3.0f);
		REQUIRE((abs(m(0, 0) - 1.0f) <= e && abs(m(0, 1) - 0.0f) <= e && abs(m(0, 2) - 0.0f) <= e
			&& abs(m(1, 0) - 0.0f) <= e && abs(m(1, 1) - 1.0f) <= e && abs(m(1, 2) - 0.0f) <= e
			&& abs(m(2, 0) - 2.0f) <= e && abs(m(2, 1) - 3.0f) <= e && abs(m(2, 2) - 1.0f) <= e));
		m = build_translation(5.0f);
		REQUIRE((abs(m(0, 0) - 1.0f) <= e && abs(m(0, 1) - 0.0f) <= e && abs(m(0, 2) - 0.0f) <= e
			&& abs(m(1, 0) - 0.0f) <= e && abs(m(1, 1) - 1.0f) <= e && abs(m(1, 2) - 0.0f) <= e
			&& abs(m(2, 0) - 5.0f) <= e && abs(m(2, 1) - 5.0f) <= e && abs(m(2, 2) - 1.0f) <= e));
	}
	SECTION("Build Rotation Matrix")
	{
		m = build_rotation(ANGLE::pi);
		REQUIRE((abs(m(0, 0) - (-1.0f)) <= e && abs(m(0, 1) - 0.0f) <= e && abs(m(0, 2) - 0.0f) <= e
			&& abs(m(1, 0) - 0.0f) <= e && abs(m(1, 1) - (-1.0f)) <= e && abs(m(1, 2) - 0.0f) <= e
			&& abs(m(2, 0) - 0.0f) <= e && abs(m(2, 1) - 0.0f) <= e && abs(m(2, 2) - 1.0f) <= e));
	}
	SECTION("Build Scaling Matrix")
	{
		m = build_scaling(3.0f, 5.0f);
		REQUIRE((abs(m(0, 0) - 3.0f) <= e && abs(m(0, 1) - 0.0f) <= e && abs(m(0, 2) - 0.0f) <= e
			&& abs(m(1, 0) - 0.0f) <= e && abs(m(1, 1) - 5.0f) <= e && abs(m(1, 2) - 0.0f) <= e
			&& abs(m(2, 0) - 0.0f) <= e && abs(m(2, 1) - 0.0f) <= e && abs(m(2, 2) - 1.0f) <= e));
		m = build_scaling(4.0f);
		REQUIRE((abs(m(0, 0) - 4.0f) <= e && abs(m(0, 1) - 0.0f) <= e && abs(m(0, 2) - 0.0f) <= e
			&& abs(m(1, 0) - 0.0f) <= e && abs(m(1, 1) - 4.0f) <= e && abs(m(1, 2) - 0.0f) <= e
			&& abs(m(2, 0) - 0.0f) <= e && abs(m(2, 1) - 0.0f) <= e && abs(m(2, 2) - 1.0f) <= e));
	}
	SECTION("Transpose")
	{
		m = transpose(build_translation(2.0f, 3.0f));
		REQUIRE((abs(m(0, 0) - 1.0f) <= e && abs(m(0, 1) - 0.0f) <= e && abs(m(0, 2) - 2.0f) <= e
			&& abs(m(1, 0) - 0.0f) <= e && abs(m(1, 1) - 1.0f) <= e && abs(m(1, 2) - 3.0f) <= e
			&& abs(m(2, 0) - 0.0f) <= e && abs(m(2, 1) - 0.0f) <= e && abs(m(2, 2) - 1.0f) <= e));
	}
}