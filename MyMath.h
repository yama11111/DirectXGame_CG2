#pragma once
#include <math.h>

// 初期化 (srand)
void InitMath();

// 乱数 (start以上 end以下)
int GetRand(const int start, const int end);

// + か, - か, 0 か
int Sign(const double num);
float SignF(const double num);

// a から b までで 割合が t のときの値
double lerp(const double a, const double b, const double t);

// 絶対値
int Abs(int a);

// 度数法 -> ラジアン
double Radian(double Angle);