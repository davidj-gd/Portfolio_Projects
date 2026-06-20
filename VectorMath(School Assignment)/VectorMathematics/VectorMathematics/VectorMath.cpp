#include "pch.h"
#include "VectorMath.h"
#include <cmath>
#include <iostream>

#define EXPORT __declspec(dllexport)
#define EXTERN extern "C"

//--------------- VECTOR3 ---------------

EXPORT Vec3 VectorAdd(Vec3 a, Vec3 b)
{
	return { a.x + b.x, a.y + b.y, a.z + b.z };
}

EXPORT Vec3 VectorSubtract(Vec3 a, Vec3 b)
{
	return { a.x - b.x, a.y - b.y, a.z - b.z };
}

EXPORT Vec3 VectorScale(Vec3 v, float scale)
{
	return { v.x * scale, v.y * scale, v.z * scale };
}

EXPORT Vec3 VectorDivide(Vec3 v, float divisor)
{
	return { v.x / divisor, v.y / divisor, v.z / divisor };
}

EXPORT float VectorMagnitude(Vec3 v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

EXPORT float VectorMagnitudeSqr(Vec3 v)
{
	return { v.x * v.x + v.y * v.y + v.z * v.z };
}

EXPORT float VectorDistance(Vec3 a, Vec3 b)
{
	return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y) + (b.z - a.z) * (b.z - a.z));
}

EXPORT Vec3 VectorNormalize(Vec3 v)
{
	float magnitude = VectorMagnitude(v);
	if (magnitude < 0.00001f) return { 0.0f, 0.0f, 0.0f };
	return { v.x / magnitude, v.y / magnitude, v.z / magnitude };
}

EXPORT float VectorDot(Vec3 a, Vec3 b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

EXPORT Vec3 VectorCross(Vec3 a, Vec3 b)
{
	return { a.y * b.z - a.z * b.y,
	a.z * b.x - a.x * b.z,
	a.x * b.y - a.y * b.x };
}

EXPORT Vec3 VectorReflect(Vec3 v, Vec3 normal)
{
	float dot = VectorDot(v, normal);
	return VectorSubtract(v, VectorScale(normal, 2.0f * dot));
	
}

EXPORT Vec3 VectorLerp(Vec3 a, Vec3 b, float t)
{
	return VectorAdd(a, VectorScale(VectorSubtract(b, a), t));
}

EXPORT float VectorAngleBetween(Vec3 a, Vec3 b)
{
	float magnitudeA = VectorMagnitude(a);
	float magnitudeB = VectorMagnitude(b);

	if (magnitudeA < 0.00001f || magnitudeB < 0.00001f)
	{
		return 0.0f;
	}

	float dot = VectorDot(a, b);
	float cosTheta = dot / (magnitudeA * magnitudeB);

	if (cosTheta > 1.0f)
	{
		cosTheta = 1.0f;
	}

	if (cosTheta < -1.0f)
	{
		cosTheta = -1.0f;
	}

	return acosf(cosTheta);
}


//--------------- VECTOR2 ---------------

EXPORT Vec2 Vector2Add(Vec2 a, Vec2 b)
{
	return { a.x + b.x, a.y + b.y };
}

EXPORT Vec2 Vector2Subtract(Vec2 a, Vec2 b)
{
	return { a.x - b.x, a.y - b.y };
}

EXPORT Vec2 Vector2Scale(Vec2 v, float scale)
{
	return { v.x * scale, v.y * scale };
}

EXPORT Vec2 Vector2Divide(Vec2 v, float divisor)
{
	return { v.x / divisor, v.y / divisor };
}

EXPORT float Vector2Magnitude(Vec2 v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

EXPORT float Vector2MagnitudeSqr(Vec2 v)
{
	return v.x * v.x + v.y * v.y;
}

EXPORT float Vector2Distance(Vec2 a, Vec2 b)
{
	return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

EXPORT Vec2 Vector2Normalize(Vec2 v)
{
	float magnitude = Vector2Magnitude(v);
	if (magnitude < 0.00001f) return { 0.0f, 0.0f };
	return { v.x / magnitude, v.y / magnitude };
}

EXPORT float Vector2Dot(Vec2 a, Vec2 b)
{
	return (a.x * b.x) + (a.y * b.y);
}

EXPORT Vec2 Vector2Reflect(Vec2 v, Vec2 normal)
{
	float dot = Vector2Dot(v, normal);
	return Vector2Subtract(v, Vector2Scale(normal, 2.0f * dot));
}

EXPORT Vec2 Vector2Lerp(Vec2 a, Vec2 b, float t)
{
	return Vector2Add(a, Vector2Scale(Vector2Subtract(b, a), t));
}

EXPORT float Vector2AngleBetween(Vec2 a, Vec2 b)
{
	float magnitudeA = Vector2Magnitude(a);
	float magnitudeB = Vector2Magnitude(b);

	if (magnitudeA < 0.00001f || magnitudeB < 0.00001f)
	{
		return 0.0f;
	}

	float dot = Vector2Dot(a, b);
	float cosTheta = dot / (magnitudeA * magnitudeB);

	if (cosTheta > 1.0f)
	{
		cosTheta = 1.0f;
	}

	if (cosTheta < -1.0f)
	{
		cosTheta = -1.0f;
	}

	return acosf(cosTheta);
}