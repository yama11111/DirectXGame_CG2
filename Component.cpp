#include "Component.h"

Component::Component() : x(0), y(0)
{
}

Component::Component(const int x, const int y) : x(x), y(y)
{
}

int Component::HalfX()
{
	return x / 2;
}

int Component::HalfY()
{
	return y / 2;
}

float Component::HalfXF()
{
	return x / 2.0f;
}

float Component::HalfYF()
{
	return y / 2.0f;
}
