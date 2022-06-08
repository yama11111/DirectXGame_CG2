#pragma once
struct Mat4
{
	float value[4][4];
	Mat4();
	constexpr Mat4(const float value[4][4]);
	constexpr Mat4 operator *(Mat4 m) const;
};

