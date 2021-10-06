struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

cbuffer colorBuffer : register(b1)
{
	float4 color;
    float4 circleParams;
}

float4 main(PS_IN input) : SV_TARGET
{
    float dist = 1 - distance(input.uv, float2(0.5, 0.5));
    dist *= smoothstep(circleParams.y, circleParams.y + 0.01, dist);

	return float4(1, 1, 1, dist) * color;
}