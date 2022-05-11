#include "Vec2.h"
#include <math.h>

double Vec2::length() const
{
	return std::sqrt(lengthSquare());
}

constexpr double Vec2::lengthSquare() const
{
	return dot(*this);
}

constexpr double Vec2::dot(const Vec2& other) const
{
	return x * other.x + y * other.y;
}

double Vec2::distanceFrom(const Vec2& other) const
{
	return (other - *this).length();
}

Vec2 Vec2::normalized() const
{
	return *this / length();
}

constexpr bool Vec2::isZero() const
{
	return x == 0.0 && y == 0.0;
}

inline constexpr Vec2 operator*(double s, const Vec2& v)
{
	return { s * v.x, s * v.y };
}