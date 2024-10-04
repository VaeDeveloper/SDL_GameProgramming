#ifndef MATH_H
#define MATMN_H

#include <cmath>	
#include <memory.h>
#include <limits>

namespace Math
{
const float Pi = 3.1415926535f;
const float TwoPi = Pi * 2.0f;
const float PiOver2 = Pi / 2.0f;
constexpr float Infinity = std::numeric_limits<float>::infinity();
constexpr float NegInfinity = -std::numeric_limits<float>::infinity();


inline float ToRadians(float degrees)
{
    return degrees * Pi / 180.0f;
}

inline float ToDegrees(float radians)
{ 
    return radians * 180.0f / Pi;
}

inline bool NearZero(float val, float epsilon = 0.001f)
{
    if (fabs(val) <= epsilon)
    {
        return true;
    }
    else
    {
        return false;
    }
}

inline float Abs(float value)
{
    return fabs(value);
}

inline float Cos(float angle)
{
    return cosf(angle);
}

inline float Sin(float angle)
{
    return sinf(angle);
}

inline float Tan(float angle)
{
    return tanf(angle);
}

inline float Acos(float value)
{
    return acosf(value);
}

inline float Atan2(float y, float x)
{
    return atan2f(y, x);
}

inline float Cot(float angle)
{
    return 1.0f / Tan(angle);
}

inline float Lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

inline float Sqrt(float value)
{
    return sqrtf(value);
}

inline float Fmod(float numer, float denom)
{
    return fmod(numer, denom);
}

template <typename T>
constexpr const T& Max(const T& a, const T& b)
{
    return (a < b ? b : a);
}

template <typename T>
constexpr const T& Min(const T& a, const T& b)
{
    return (a < b ? a : b);
}

template <typename T>
constexpr const T& Clamp(const T& val, const T& lower, const T& upper)
{
    return Min(upper, Max(lower, val));
}

}

#endif
