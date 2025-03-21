#ifndef SAL_MATH_H
#define SAL_MATH_H

#define PI 3.14159265359

struct Rect
{
    float x;
    float y;
    float w;
    float h;
};

struct Vector2
{
    float x;
    float y;
};

inline Vector2 operator+(const Vector2& a, const Vector2& b)
{
    return { a.x + b.x, a.y + b.y };
}

inline Vector2& operator+=(Vector2& a, const Vector2& b)
{
    a.x += b.x;
    a.y += b.y;

    return a;
}

inline Vector2 operator-(const Vector2& a, const Vector2& b)
{
    return { a.x - b.x, a.y - b.y };
}

inline Vector2& operator-=(Vector2& a, const Vector2& b)
{
    a.x -= b.x;
    a.y -= b.y;

    return a;
}

inline Vector2 operator*(const Vector2& v, float s)
{
    return { v.x * s, v.y * s };
}

inline Vector2& operator*=(Vector2& v, float s)
{
    v.x *= s;
    v.y *= s;

    return v;
}

#endif