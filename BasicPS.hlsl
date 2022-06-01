#include "Basic.hlsli"

//cbuffer ConstBufferDataMaterial : register(b0) 
//{
//	float4 color; // 色(RGBA)
//}

//float4 main() : SV_TARGET
//{
//	return color;
//}

//float4 main(VSOutput input) : SV_TARGET
//{
//	return float4(input.uv, 0, 1);
//}

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	return float4(tex.Sample(smp, input.uv)) * 3;
}