using UnityEngine;
using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential)]
public struct Vec2
{
    public float x;
    public float y;
}

public struct Vec3
{
    public float x;
    public float y;
    public float z;
}

public static class VectorMath
{
    private const string DLL_NAME = "VectorMathematics";


    //------------ VECTOR 2 FUNCTIONS -------------//

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern Vec2 Vector2Add(Vec2 a, Vec2 b);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern Vec2 Vector2Subtract(Vec2 a, Vec2 b);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern Vec2 Vector2Scale(Vec2 v, float scale);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern Vec2 Vector2Divide(Vec2 v, float divisor);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern float Vector2Magnitude(Vec2 v);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern float Vector2MagnitudeSqr(Vec2 v);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern float Vector2Distance(Vec2 a, Vec2 b);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern Vec2 Vector2Normalize(Vec2 v);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern float Vector2Dot(Vec2 a, Vec2 b);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern Vec2 Vector2Reflect(Vec2 v, Vec2 normal);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern Vec2 Vector2Lerp(Vec2 a, Vec2 b, float t);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern float Vector2AngleBetween(Vec2 a, Vec2 b);


    //------------ VECTOR 3 FUNCTIONS -------------//


    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern Vec3 VectorAdd(Vec3 a, Vec3 b);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern Vec3 VectorSubtract(Vec3 a, Vec3 b);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern Vec3 VectorScale(Vec3 v, float scale);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern Vec3 VectorDivide(Vec3 v, float divisor);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern float VectorMagnitude(Vec3 v);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern float VectorMagnitudeSqr(Vec3 v);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern float VectorDistance(Vec3 a, Vec3 b);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern Vec3 VectorNormalize(Vec3 v);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern float VectorDot(Vec3 a, Vec3 b);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern Vec3 VectorCross(Vec3 a, Vec3 b);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern Vec3 VectorReflect(Vec3 v, Vec3 normal);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern Vec3 VectorLerp(Vec3 a, Vec3 b, float t);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern float VectorAngleBetween(Vec3 a, Vec3 b);

}