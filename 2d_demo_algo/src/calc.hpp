#pragma once

#include <cmath>
#include "types.hpp"

inline float2 operator+(float2 a, float2 b) { return { a.x + b.x, a.y + b.y }; }
inline float2 operator-(float2 a, float2 b) { return { a.x - b.x, a.y - b.y }; }
inline float2 operator*(float2 a, float2 b) { return { a.x * b.x, a.y * b.y }; }
inline float2 operator/(float2 a, float2 b) { return { a.x / b.x, a.y / b.y }; }

inline float2 operator+(float2 a, float b) { return { a.x + b, a.y + b }; }
inline float2 operator-(float2 a, float b) { return { a.x - b, a.y - b }; }
inline float2 operator*(float2 a, float b) { return { a.x * b, a.y * b }; }
inline float2 operator/(float2 a, float b) { return { a.x / b, a.y / b }; }

inline float2& operator+=(float2& a, float2 b) { return a = a + b; return a; }
inline float2& operator-=(float2& a, float2 b) { return a = a - b; return a; }
inline float2& operator*=(float2& a, float2 b) { return a = a * b; return a; }
inline float2& operator/=(float2& a, float2 b) { return a = a / b; return a; }

namespace calc
{
    static const float TAU = 6.283185307179586f;

    template <typename T>
    inline T lerp(T a, T b, float t)
    {
        return a * (1 - t) + b * t;
    }

    inline float inverseLerp(float a, float b, float t)
    {
        return (t - a) / (b - a);
    }

    inline float remap(float iMin, float iMax, float oMin, float oMax, float value)
    {
        float t = inverseLerp(iMin, iMax, value);
        return lerp(oMin, oMax, t);
    }

    inline float length(float2 v)
    {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }

    inline float lengthSq(float2 v)
    {
        return v.x * v.x + v.y * v.y;
    }

    inline int trueMod(int a, int m)
    {
        int r = a % m;
        return (r >= 0) ? r : r + m;
    }

    inline float dot(float2 a, float2 b)
    {
        return a.x * b.x + a.y * b.y;
    }
}
