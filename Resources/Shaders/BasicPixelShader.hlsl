struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

cbuffer colorBuffer : register(b1)
{
	float4 color;
}

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(PS_IN input) : SV_TARGET
{
	return tex.Sample(smp, input.uv) * color;
}