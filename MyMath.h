#pragma once
#include <math.h>

// ������ (srand)
void InitMath();

// ���� (start�ȏ� end�ȉ�)
int GetRand(const int start, const int end);

// + ��, - ��, 0 ��
int Sign(const double num);
float SignF(const double num);

// a ���� b �܂ł� ������ t �̂Ƃ��̒l
double lerp(const double a, const double b, const double t);

// ��Βl
int Abs(int a);

// �x���@ -> ���W�A��
double Radian(double Angle);

