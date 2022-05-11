#pragma once

struct Component
{
public:
	int x;
	int y;
public:
	Component();
	Component(const int x, const int y);
	int HalfX();
	int HalfY();
	float HalfXF();
	float HalfYF();
};

