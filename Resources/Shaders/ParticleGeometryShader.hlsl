struct GS_IN
{
	float4 pos : SV_POSITION;
};

struct GS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

cbuffer wvpBuffer : register(b0)
{
	float4x4 wvp;
}

static const float4 offset_Array[4] =
{
	float4(-0.5f, -0.5f, 0.0f, 0.0f),	//左下
	float4(-0.5f,  0.5f, 0.0f, 0.0f),	//左上
	float4( 0.5f, -0.5f, 0.0f, 0.0f),	//右下
	float4( 0.5f,  0.5f, 0.0f, 0.0f)	//右上
};

static const float2 uv_Array[4] =
{
	float2(0, 1),
	float2(0, 0),
	float2(1, 1),
	float2(1, 0)
};

[maxvertexcount(4)]
void main(
	point GS_IN input[1], 
	inout TriangleStream<GS_OUT> output
)
{
	GS_OUT element;

	for (uint i = 0; i < 4; i++)
	{
		element.pos = input[0].pos + offset_Array[i];
		element.pos = mul(element.pos, wvp);
		element.uv = uv_Array[i];
		output.Append(element);
	}
}