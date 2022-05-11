#pragma once
#include <iostream>
using namespace std;

struct Vec2
{
    double x;
    double y;

    Vec2() = default;
    constexpr Vec2(double x, double y)
        :x(x), y(y)
    {}

    double length() const; // í∑Ç≥
    constexpr double lengthSquare() const; // í∑Ç≥ 2èÊ
    constexpr double dot(const Vec2& other) const; // ì‡êœ
    double distanceFrom(const Vec2& other) const; // ãóó£
    Vec2 normalized() const; // ê≥ãKâª
    constexpr bool isZero() const; // óÎÉxÉNÉgÉãÇ©

    constexpr Vec2 operator +() const
    {
        return *this;
    }
    constexpr Vec2 operator -() const
    {
        return { -x,-y };
    }
    constexpr Vec2 operator +(const Vec2& other) const
    {
        return{ x + other.x, y + other.y };
    }
    constexpr Vec2 operator -(const Vec2& other) const
    {
        return{ x - other.x, y - other.y };
    }
    constexpr Vec2 operator *(double s) const
    {
        return{ x * s, y * s };
    }
    constexpr Vec2 operator /(double s) const
    {
        return{ x / s, y / s };
    }
    constexpr Vec2& operator +=(const Vec2& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
    constexpr Vec2& operator -=(const Vec2& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    constexpr Vec2& operator *=(double s)
    {
        x *= s;
        y *= s;
        return *this;
    }
    constexpr Vec2& operator /=(double s)
    {
        x /= s;
        y /= s;
        return *this;
    }
};

inline constexpr Vec2 operator *(double s, const Vec2& v);

template <class Char>
inline std::basic_ostream<Char>& operator <<(std::basic_ostream<Char>& os, const Vec2& v)
{
    return os << Char('(') << v.x << Char(',') << v.y << Char(')');
}

template <class Char>
inline std::basic_istream<Char>& operator >>(std::basic_istream<Char>& is, Vec2& v)
{
    Char unused;
    return is >> unused >> v.x >> unused >> v.y >> unused;
}
