#include "DefaultInclude.hlsli"

struct VS_IN
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD;
	float4 normal : NORMAL;
	uint4 boneIndices : BONEINDICES;
	float4 boneWeights : BONEWEIGHTS;

	//インスタンシング用
	float4x4 instanceMat : INSTANCEMAT;
	float4 instanceColor : INSTANCECOLOR;
	float4x3 instanceUVOrigins : INSTANCEUVORIGINS;
	float4x3 instanceUVSizes : INSTANCEUVSIZES;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
	float4 color : TEXCOORD2;
};

shared static int vertexCount = 12;
shared static float2 uvMultPatterns[6] = 
{
	//左上
	float2(0.0, 0.0),
	//右下
	float2(1.0, 1.0),
	//右上
	float2(1.0, 0.0),
	//右上
	float2(1.0, 0.0),
	//左上
	float2(0.0, 0.0),
	//左下
	float2(0.0, 1.0)
};

VS_OUT main(VS_IN input)
{
	VS_OUT output;
	//インスタンス用行列 * view * proj
	float4x4 mat = mul(mul(input.instanceMat, view), projection);
	output.pos = mul(input.pos, mat);
	output.normal = normalize(mul(input.normal, (float3x3)input.instanceMat));

	int uvIndex = vertexCount / 12;
	int indexOffset = step(uvIndex, 3) * 2;
	int multPatternIndex = vertexCount % 6;

	vertexCount++;
	vertexCount = vertexCount % 35;

	//UVの原点 + 頂点毎のUVパターン * UVのサイズ
	float2 uvOrigin = float2(input.instanceUVOrigins[indexOffset][uvIndex], input.instanceUVOrigins[1 + indexOffset][uvIndex]);
	float2 uvSize = float2(input.instanceUVSizes[indexOffset][uvIndex], input.instanceUVSizes[1 + indexOffset][uvIndex]);

	output.uv = uvOrigin + uvMultPatterns[multPatternIndex] * uvSize;

	output.worldPos = mul(input.pos, input.instanceMat).xyz;
	output.color = input.instanceColor;

	return output;
}