struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

static const int sampleCount = 32;

cbuffer gaussianBlurBuffer : register(b1)
{
	float4 offset;
	float4 weights[sampleCount];
}

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(PS_IN input) : SV_TARGET
{
	float4 result = float4(0, 0, 0, 0);

	//近くのピクセルから色を取得
	for (int i = sampleCount - 1; i > 0; i--)
	{
		float2 off = offset.xy * i;
		result += tex.Sample(smp, input.uv - off) * weights[i].x;
	}

	for (int i = 0; i < sampleCount; i++)
	{
		float2 off = offset.xy * i;
		result += tex.Sample(smp, input.uv + off) * weights[i].x;
	}

	return result;
}