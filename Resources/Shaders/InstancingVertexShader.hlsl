#include "DefaultInclude.hlsli"

struct VS_IN
{
	float4 pos : POSITION;
	float4 normal : NORMAL;
	float2 uv : TEXCOORD;
	uint4 boneIndices : BONEINDICES;
	float4 boneWeights : BONEWEIGHTS;

	//インスタンシング用
	float4x4 instanceMat : INSTANCEMAT;
	float4 instanceColor : INSTANCECOLOR;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
	float4 color : TEXCOORD2;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output;
	//インスタンス用行列 * view * proj
	float4x4 mat = mul(mul(input.instanceMat, view), projection);
	output.pos = mul(input.pos, mat);
	output.normal = normalize(mul(input.normal, (float3x3)input.instanceMat));
	output.uv = input.uv;
	output.worldPos = mul(input.pos, input.instanceMat).xyz;
	output.color = input.instanceColor;
	return output;
}