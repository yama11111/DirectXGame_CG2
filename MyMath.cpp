#include "MyMath.h"
#include <stdlib.h>
#include <time.h>
#define PI 3.14159265359

void InitMath()
{
	srand(time(NULL));
}

int GetRand(const int start, const int end)
{
	return ((rand() % (end + 1)) + start);
}

int Sign(const double num)
{
	if (num > 0) return 1;
	if (num < 0) return -1;
	return 0;
}
float SignF(const double num)
{
	if (num > 0) return 1.0f;
	if (num < 0) return -1.0f;
	return 0.0f;
}

double lerp(const double a, const double b, const double t)
{
	return a + t * (b - a);
}

// ----- BelugaLib ----- //
int Abs(int a)
{
	if (a < 0)
	{
		a = -a;
	}
	return a;
}

double Radian(double Angle)
{
	return Angle * PI / 180;
}
// ---------------------//