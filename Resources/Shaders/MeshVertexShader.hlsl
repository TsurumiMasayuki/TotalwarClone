#include "DefaultInclude.hlsli"

struct VS_IN
{
	float4 pos : POSITION;
	float4 normal : NORMAL;
	float2 uv : TEXCOORD;
	uint4 boneIndices : BONEINDICES;
	float4 boneWeights : BONEWEIGHTS;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
};

struct SkinOutput
{
	float4 pos;
	float3 normal;
};

cbuffer skinBuffer : register(b1)
{
	float4x4 skinning[32];
};

SkinOutput ComputeSkin(VS_IN input)
{
	//ゼロクリア
	SkinOutput output = (SkinOutput)0;

	uint iBone;		//計算するボーン番号
	float weight;	//ボーンウェイト
	float4x4 m;		//スキニング行列

	//ボーン0
	iBone = input.boneIndices.x;
	weight = input.boneWeights.x;
	m = skinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, (float3)input.normal);
	
	//ボーン1
	iBone = input.boneIndices.y;
	weight = input.boneWeights.y;
	m = skinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, (float3)input.normal);
	
	//ボーン2
	iBone = input.boneIndices.z;
	weight = input.boneWeights.z;
	m = skinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, (float3)input.normal);

	//ボーン3
	iBone = input.boneIndices.w;
	weight = input.boneWeights.w;
	m = skinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, (float3)input.normal);
	
	return output;
}

VS_OUT main(VS_IN input)
{
	//スキニング計算
	SkinOutput skinned = ComputeSkin(input);

	VS_OUT output;
	output.pos = mul(skinned.pos, wvp);
	output.normal = normalize(mul(skinned.normal, (float3x3)world));
	output.uv = input.uv;
	output.worldPos = mul(input.pos, world).xyz;
	return output;
}