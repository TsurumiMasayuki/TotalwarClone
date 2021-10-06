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
	float4 sampled1 = tex1.Sample(smp, input.uv);
	float4 sampled2 = tex2.Sample(smp, input.uv);
	return sampled1 + sampled2;
}