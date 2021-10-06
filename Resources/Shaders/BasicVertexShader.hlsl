#include "DefaultInclude.hlsli"

struct VS_IN
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output;
	output.pos = mul(input.pos, wvp);
	output.uv = input.uv;
	return output;
}