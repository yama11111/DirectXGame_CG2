#include "Basic.hlsli"

//cbuffer ConstBufferDataMaterial : register(b0) 
//{
//	float4 color; // �F(RGBA)
//}

//float4 main() : SV_TARGET
//{
//	return color;
//}

float4 main(VSOutput input) : SV_TARGET
{
	return float4(input.uv, 0, 1);
}