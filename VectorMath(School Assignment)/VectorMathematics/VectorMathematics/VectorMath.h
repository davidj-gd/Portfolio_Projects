#pragma once

#ifdef _WIN64
#define EXPORT __declspec(dllexport)
#endif

struct Vec2
{
	float x;
	float y;
};

struct Vec3
{
	float x;
	float y;
	float z;
};

extern "C"
{
	EXPORT Vec3 VectorAdd(Vec3 a, Vec3 b);
	EXPORT Vec3 VectorSubtract(Vec3 a, Vec3 b);
	EXPORT Vec3 VectorScale(Vec3 v, float scale);
	EXPORT Vec3 VectorDivide(Vec3 v, float divisor);
	EXPORT float VectorMagnitude(Vec3 v);
	EXPORT float VectorMagnitudeSqr(Vec3 v);
	EXPORT float VectorDistance(Vec3 a, Vec3 b);
	EXPORT Vec3 VectorNormalize(Vec3 v);
	EXPORT float VectorDot(Vec3 a, Vec3 b);
	EXPORT Vec3 VectorCross(Vec3 a, Vec3 b);
	EXPORT Vec3 VectorReflect(Vec3 v, Vec3 normal);
	EXPORT Vec3 VectorLerp(Vec3 a, Vec3 b, float t);
	EXPORT float VectorAngleBetween(Vec3 a, Vec3 b);

	EXPORT Vec2 Vector2Add(Vec2 a, Vec2 b);
	EXPORT Vec2 Vector2Subtract(Vec2 a, Vec2 b);
	EXPORT Vec2 Vector2Scale(Vec2 v, float scale);
	EXPORT Vec2 Vector2Divide(Vec2 v, float divisor);
	EXPORT float Vector2Magnitude(Vec2 v);
	EXPORT float Vector2MagnitudeSqr(Vec2 v);
	EXPORT float Vector2Distance(Vec2 a, Vec2 b);
	EXPORT Vec2 Vector2Normalize(Vec2 v);
	EXPORT float Vector2Dot(Vec2 a, Vec2 b);
	EXPORT Vec2 Vector2Reflect(Vec2 v, Vec2 normal);
	EXPORT Vec2 Vector2Lerp(Vec2 a, Vec2 b, float t);
	EXPORT float Vector2AngleBetween(Vec2 a, Vec2 b);

}
