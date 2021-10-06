struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

Texture2D<float4> tex1 : register(t0);
Texture2D<float4> tex2 : register(t1);
SamplerState smp : register(s0);

float4 main(PS_IN input) : SV_TARGET
{
	float2 uv = input.uv;
	float4 sampled1 = tex1.Sample(smp, uv);
	float4 sampled2 = tex2.Sample(smp, uv);

	float4 result = sampled1;

	if (fmod(input.uv.y, 0.1f) < 0.05f)
	{
		result = sampled2;
	}

	return result;
}