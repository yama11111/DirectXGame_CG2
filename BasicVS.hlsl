#include "Basic.hlsli"

//float4 main(float4 pos : POSITION) : SV_POSITION
//{
//	//return pos + float4(0.5f, 0.5f, 0, 0);
//	//return pos + float4(0.5f, 0.5f, 1, 1);
//	//float4 pos2 = pos;
//	//const float PI = 3.14159;
//	//float angle = PI * 0.25f;
//	//pos2.x = pos.x * cos(angle) - pos.y * sin(angle);
//	//pos2.y = pos.x * sin(angle) - pos.y * cos(angle);
//	//return pos2;
//	return pos;
//}

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	VSOutput output; // ピクセルシェーダーに渡す値
	output.svpos = pos;
	output.uv = uv;
	return output;
}