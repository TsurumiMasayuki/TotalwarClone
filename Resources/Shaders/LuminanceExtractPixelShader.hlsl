struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

//輝度の閾値
static float luminanceThreshold = 0.6;
Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

//輝度計算
float GetLuminance(float3 rgb)
{
	return rgb.x * 0.299 + rgb.y * 0.587 + rgb.z * 0.114;
}

float4 main(PS_IN input) : SV_TARGET
{
	float4 sampled = tex.Sample(smp, input.uv);
	float luminance = GetLuminance(sampled.xyz);
	//輝度が閾値より高かったら通す
	return float4(sampled.xyz * step(luminanceThreshold, luminance), sampled.w);
}