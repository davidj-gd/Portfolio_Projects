// VectorMathematicsTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "VectorMath.h"
#include <cmath>
#include <cassert>

bool FloatEqual(float a, float b, float tolerance = 0.0001f)
{
	return fabs(a - b) < tolerance;
}

//------------ VECTOR 3 (Tests) --------------//

void TestVec3Add()
{
	std::cout << "Testing VectorAdd..." << std::endl;

	Vec3 a = { 1.0f, 2.0f, 7.0f };
	Vec3 b = { 3.0f, 4.0f, 1.0f };
	Vec3 result = VectorAdd(a, b);

	assert(FloatEqual(result.x, 4.0f) && "VectorAdd X component failed");
	assert(FloatEqual(result.y, 6.0f) && "VectorAdd Y component failed");
	assert(FloatEqual(result.z, 8.0f) && "VectorAdd Z component failed");

	std::cout << " Passed!" << std::endl;
}

void TestVec3Subtract()
{
	std::cout << "Testing VectorSubtract..." << std::endl;

	Vec3 a = { 5.0f, 6.0f, 7.0f };
	Vec3 b = { 3.0f, 4.0f, 1.0f };
	Vec3 result = VectorSubtract(a, b);

	assert(FloatEqual(result.x, 2.0f) && "VectorSub X component failed");
	assert(FloatEqual(result.y, 2.0f) && "VectorSub Y component failed");
	assert(FloatEqual(result.z, 6.0f) && "VectorSub Z component failed");

	std::cout << " Passed!" << std::endl;
}

void TestVec3Scale()
{
	std::cout << "Testing VectorScale..." << std::endl;

	Vec3 a = { 5.0f, 2.0f, 3.0f };
	float scale = { 2.0f };
	Vec3 result = VectorScale(a, scale);

	assert(FloatEqual(result.x, 10.0f) && "VectorScale X component failed");
	assert(FloatEqual(result.y, 4.0f) && "VectorScale Y component failed");
	assert(FloatEqual(result.z, 6.0f) && "VectorScale Z component failed");

	std::cout << " Passed!" << std::endl;
}

void TestVec3Magnitude()
{
	std::cout << "Testing VectorMagnitude..." << std::endl;

	Vec3 v = { 0.0f, 0.0f, 4.0f };
	float magnitude = VectorMagnitude(v);

	assert(FloatEqual(magnitude, 4.0f) && "VectorMagnitude failed");

	std::cout << "  Magnitude: " << magnitude << std::endl;
	std::cout << " Passed!" << std::endl;
}

void TestVec3MagnitudeSqrt()
{
	std::cout << "Testing VectorMagnitudeSqr..." << std::endl;

	Vec3 v = { 2.0f, 2.0f, 2.0f };
	float result = VectorMagnitudeSqr(v);

	assert(FloatEqual(result, 12.0f) && "VectorMagSqrt failed");

	std::cout << "  Magnitude: " << result << std::endl;
	std::cout << " Passed!" << std::endl;
}

void TestVec3Distance()
{
	std::cout << "Testing VectorDistance..." << std::endl;

	Vec3 a = { 0.0f, 0.0f, 0.0f };
	Vec3 b = { 3.0f, 4.0f, 0.0f };
	float result = VectorDistance(a, b);

	assert(FloatEqual(result, 5.0f) && "VectorDistance failed");

	std::cout << "  Distance: " << result << std::endl;
	std::cout << " Passed!" << std::endl;
}

void TestVec3Normalize()
{
	std::cout << "Testing VectorNormalize..." << std::endl;

	Vec3 v = { 0.0f, 5.0f, 4.0f };
	Vec3 n = VectorNormalize(v);

	float magnitude = VectorMagnitude(n);

	assert(FloatEqual(magnitude, 1.0f) && "VectorNormalize is not 1");

	assert(FloatEqual(n.x, 0.0f) && "VectorNormalize X failed");
	assert(FloatEqual(n.y, 0.6f) && "VectorNormalize Y failed");
	assert(FloatEqual(n.z, 0.8f) && "VectorNormalize Z failed");

	Vec3 zero = { 0.0f, 0.0f, 0.0f };
	Vec3 nZero = VectorNormalize(zero);
	assert(FloatEqual(nZero.x, 0.0f) && "normalize zero X");
	assert(FloatEqual(nZero.y, 0.0f) && "normalize zero Y");
	assert(FloatEqual(nZero.z, 0.0f) && "normalize zero Z");

	std::cout << "Passed!" << std::endl;

}

void TestVec3Dot()
{
	std::cout << "Testing VectorDot..." << std::endl;

	Vec3 a = { 1.0f, 0.0f, 0.0f };
	Vec3 b = { 2.0f, 0.0f, 0.0f };
	float dot1 = VectorDot(a, b);
	assert(FloatEqual(dot1, 2.0f) && "Dot same direcitn failed");

	Vec3 c = { 0.0f, 1.0f, 0.0f };
	float dot2 = VectorDot(a, c);
	assert(FloatEqual(dot1, 0.0f) && "Dot perpendicular failed");

	Vec3 d = { -1.0f, 0.0f, 0.0f };
	float dot3 = VectorDot(a, d);
	assert(FloatEqual(dot3, -1.0f) && "Dot opposite failed");

	std::cout << "Passed!" << std::endl;
}

void TestVec3Cross()
{
	std::cout << "Testing VectorCross.." << std::endl;

	Vec3 i = { 1.0f, 0.0f, 0.0f };
	Vec3 j = { 0.0f, 1.0f, 0.0f };
	Vec3 k = { 0.0f, 0.0f, 1.0f };

	Vec3 result1 = VectorCross(i, j);
	assert(FloatEqual(result1.x, 0.0f) && "Cross i x j X failed");
	assert(FloatEqual(result1.y, 0.0f) && "Cross i x j Y failed");
	assert(FloatEqual(result1.z, 1.0f) && "Cross i x j Z failed");

	Vec3 result2 = VectorCross(j, k);
	assert(FloatEqual(result2.x, 1.0f));
	assert(FloatEqual(result2.y, 0.0f));
	assert(FloatEqual(result2.z, 0.0f));

	std::cout << "Passed!" << std::endl;
}

void TestVec3Reflect()
{
	std::cout << "Testing VectorReflect.." << std::endl;

	Vec3 v1 = { 0.0f, -1.0f, 0.0f };
	Vec3 normal = { 0.0f, 1.0f, 0.0f };
	Vec3 reflect = VectorReflect(v1, normal);
	assert(FloatEqual(reflect.x, 0.0f));
	assert(FloatEqual(reflect.y, 1.0f));
	assert(FloatEqual(reflect.z, 0.0f));

	Vec3 v2 = { 1.0f, -1.0f, 0.0f };
	Vec3 reflect2 = VectorReflect(v2, normal);
	assert(FloatEqual(reflect2.x, 1.0f));
	assert(FloatEqual(reflect2.y, 1.0f));
	assert(FloatEqual(reflect2.z, 0.0f));

	std::cout << "Passed!" << std::endl;
}

void TestVec3Lerp()
{
	std::cout << "Testing VectorLerp.." << std::endl;

	Vec3 a = { 0.0f, 0.0f, 0.0f };
	Vec3 b = { 2.0f, 4.0f, 6.0f };

	Vec3 result0 = VectorLerp(a, b, 0.0f);
	assert(FloatEqual(result0.x, a.x));
	assert(FloatEqual(result0.y, a.y));
	assert(FloatEqual(result0.z, a.z));

	Vec3 result1 = VectorLerp(a, b, 1.0f);
	assert(FloatEqual(result1.x, b.x));
	assert(FloatEqual(result1.y, b.y));
	assert(FloatEqual(result1.z, b.z));

	Vec3 resultHalf = VectorLerp(a, b, 0.5f);
	assert(FloatEqual(resultHalf.x, 1.0f));
	assert(FloatEqual(resultHalf.y, 2.0f));
	assert(FloatEqual(resultHalf.z, 3.0f));

	std::cout << " Passed!" << std::endl;
}

void TestVec3AngleBetween()
{
	std::cout << "Testing VectorAngleBetwen.." << std::endl;

	Vec3 a = { 1.0f, 0.0f, 0.0f };
	Vec3 b = { 1.0f, 0.0f, 0.0f };
	float angleZero = VectorAngleBetween(a, b);
	assert(FloatEqual(angleZero, 0.0f));

	Vec3 c = { 0.0f, 1.0f, 0.0f };
	float angle90 = VectorAngleBetween(a, c);
	float piOver2 = 3.14159265f / 2.0f;
	assert(FloatEqual(angle90, piOver2));

	Vec3 d = { -1.0f, 0.0f, 0.0f };
	float angle180 = VectorAngleBetween(a, d);
	float pi = 3.14159265f;
	assert(FloatEqual(angle180, pi));

	Vec3 zero = { 0.0f, 0.0f, 0.0f };
	float noAngle = VectorAngleBetween(a, zero);
	assert(FloatEqual(noAngle, 0.0f));

	std::cout << "Passed!" << std::endl;

}

//------------ VECTOR 2 (Tests) --------------//

void TestVec2Add()
{
	std::cout << "Testing Vector2Add..." << std::endl;

	Vec2 a = { 1.0f, 2.0f };
	Vec2 b = { 3.0f, 4.0f };
	Vec2 result = Vector2Add(a, b);

	assert(result.x == 4.0f && "Vector2Add X component failed");
	assert(result.y == 6.0f && "Vector2Add Y component failed");

	std::cout << " Passed!" << std::endl;
}

void TestVec2Subtract()
{
	std::cout << "Testing Vector2Subtract..." << std::endl;

	Vec2 a = { 5.0f, 6.0f };
	Vec2 b = { 3.0f, 4.0f };
	Vec2 result = Vector2Subtract(a, b);

	assert(FloatEqual(result.x, 2.0f) && "Vector2Sub X component failed");
	assert(FloatEqual(result.y, 2.0f) && "Vector2Sub Y component failed");

	std::cout << " Passed!" << std::endl;
}

void TestVec2Scale()
{
	std::cout << "Testing Vector2Scale..." << std::endl;

	Vec2 a = { 5.0f, 2.0f };
	float scale = { 2.0f };
	Vec2 result = Vector2Scale(a, scale);

	assert(FloatEqual(result.x, 10.0f) && "Vector2Scale X component failed");
	assert(FloatEqual(result.y, 4.0f) && "Vector2Scale Y component failed");

	std::cout << " Passed!" << std::endl;
}

void TestVec2Magnitude()
{
	std::cout << "Testing Vector2Magnitude..." << std::endl;

	Vec2 v = { 0.0f, 4.0f };
	float magnitude = Vector2Magnitude(v);

	assert(FloatEqual(magnitude, 4.0f) && "VectorMagnitude failed");

	std::cout << "  Magnitude: " << magnitude << std::endl;
	std::cout << " Passed!" << std::endl;
}

void TestVec2MagnitudeSqrt()
{
	std::cout << "Testing Vector2MagnitudeSqr..." << std::endl;

	Vec2 v = { 2.0f, 2.0f };
	float result = Vector2MagnitudeSqr(v);

	assert(FloatEqual(result, 8.0f) && "VectorMagSqrt failed");

	std::cout << "  Magnitude: " << result << std::endl;
	std::cout << " Passed!" << std::endl;
}

void TestVec2Distance()
{
	std::cout << "Testing Vector2Distance..." << std::endl;

	Vec2 a = { 0.0f, 0.0f };
	Vec2 b = { 3.0f, 4.0f };
	float result = Vector2Distance(a, b);

	assert(FloatEqual(result, 5.0f) && "Vector2Distance failed");

	std::cout << " Distance: " << result << std::endl;
	std::cout << " Passed!" << std::endl;
}

void TestVec2Normalize()
{
	std::cout << "Testing Vector2Normalize..." << std::endl;

	Vec2 v = { 0.0f, 4.0f };
	Vec2 n = Vector2Normalize(v);

	float magnitude = Vector2Magnitude(n);

	assert(FloatEqual(magnitude, 1.0f) && "Vector2Normalize is not 1");

	assert(FloatEqual(n.x, 0.0f) && "Vector2Normalize X failed");
	assert(FloatEqual(n.y, 1.0f) && "Vector2Normalize Y failed");

	Vec2 zero = { 0.0f, 0.0f };
	Vec2 nZero = Vector2Normalize(zero);
	assert(FloatEqual(nZero.x, 0.0f) && "normalize zero X");
	assert(FloatEqual(nZero.y, 0.0f) && "normalize zero Y");

	std::cout << "Passed!" << std::endl;
}

void TestVec2Dot()
{
	std::cout << "Testing Vector2Dot..." << std::endl;

	Vec2 a = { 1.0f, 0.0f };
	Vec2 b = { 2.0f, 0.0f };
	float dot1 = Vector2Dot(a, b);
	assert(FloatEqual(dot1, 2.0f) && "Vector2Dot same direction failed");

	Vec2 c = { 0.0f, 1.0f };
	float dot2 = Vector2Dot(a, c);
	assert(FloatEqual(dot1, 0.0f) && "Vector2Dot perpendicular failed");

	Vec2 d = { -1.0f, 0.0f };
	float dot3 = Vector2Dot(a, d);
	assert(FloatEqual(dot3, -1.0f) && "Vector2Dot opposite failed");

	std::cout << "Passed!" << std::endl;
}

void TestVec2Reflect()
{
	std::cout << "Testing Vector2Reflect.." << std::endl;

	Vec2 normal = { 0.0f, 1.0f };

	Vec2 v1 = { 0.0f, -1.0f };
	
	Vec2 reflect = Vector2Reflect(v1, normal);
	assert(FloatEqual(reflect.x, 0.0f));
	assert(FloatEqual(reflect.y, 1.0f));

	Vec2 v2 = { 1.0f, -1.0f };
	Vec2 reflect2 = Vector2Reflect(v2, normal);
	assert(FloatEqual(reflect2.x, 1.0f));
	assert(FloatEqual(reflect2.y, 1.0f));

	std::cout << "Passed!" << std::endl;
}

void TestVec2Lerp()
{
	std::cout << "Testing Vector2Lerp.." << std::endl;

	Vec2 a = { 0.0f, 0.0f };
	Vec2 b = { 2.0f, 4.0f };

	Vec2 result0 = Vector2Lerp(a, b, 0.0f);
	assert(FloatEqual(result0.x, a.x));
	assert(FloatEqual(result0.y, a.y));

	Vec2 result1 = Vector2Lerp(a, b, 1.0f);
	assert(FloatEqual(result1.x, b.x));
	assert(FloatEqual(result1.y, b.y));

	Vec2 resultHalf = Vector2Lerp(a, b, 0.5f);
	assert(FloatEqual(resultHalf.x, 1.0f));
	assert(FloatEqual(resultHalf.y, 2.0f));

	std::cout << " Passed!" << std::endl;
}

void TestVec2AngleBetween()
{
	std::cout << "Testing VectorAngleBetwen.." << std::endl;

	Vec2 a = { 1.0f, 0.0f };
	Vec2 b = { 1.0f, 0.0f };
	float angleZero = Vector2AngleBetween(a, b);
	assert(FloatEqual(angleZero, 0.0f));

	Vec2 c = { 0.0f, 1.0f };
	float angle90 = Vector2AngleBetween(a, c);
	float piOver2 = 3.14159265f / 2.0f;
	assert(FloatEqual(angle90, piOver2));

	Vec2 d = { -1.0f, 0.0f };
	float angle180 = Vector2AngleBetween(a, d);
	float pi = 3.14159265f;
	assert(FloatEqual(angle180, pi));

	Vec2 zero = { 0.0f, 0.0f };
	float noAngle = Vector2AngleBetween(a, zero);
	assert(FloatEqual(noAngle, 0.0f));

	std::cout << "Passed!" << std::endl;
}

int main()
{
	// all tests passed

	TestVec3Add();
	TestVec3Subtract();
	TestVec3Scale();
	TestVec3Magnitude();
	TestVec3MagnitudeSqrt();
	TestVec3Distance();
	TestVec3Normalize();
	TestVec3Dot();
	TestVec3Cross();
	TestVec3Reflect();
	TestVec3Lerp();
	TestVec3AngleBetween();

	TestVec2Add();
	TestVec2Subtract();
	TestVec2Scale();
	TestVec2Magnitude();
	TestVec2MagnitudeSqrt();
	TestVec2Distance();
	TestVec2Normalize();
	TestVec2Dot();
	TestVec2Reflect();
	TestVec2Lerp();
	TestVec2AngleBetween();
}


