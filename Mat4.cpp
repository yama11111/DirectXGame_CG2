#include "Mat4.h"

Mat4::Mat4()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++) 
		{
			value[i][j] = 0;
		}
	}
}

Mat4::Mat4(const float value[4][4])
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			this->value[i][j] = value[i][j];
		}
	}
}

Mat4 Mat4::operator*(Mat4& m)
{
	Mat4 mat;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				mat.value[i][j] += m.value[i][k] * m.value[k][j];
			}
		}
	}
	return mat;
}