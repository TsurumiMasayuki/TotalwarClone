#include "DefaultInclude.hlsli"

struct VS_IN
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD;
	float4 normal : NORMAL;
	uint4 boneIndices : BONEINDICES;
	float4 boneWeights : BONEWEIGHTS;

	uint vertexID : SV_VERTEXID;

	uint instanceID : SV_INSTANCEID;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
	float4 color : TEXCOORD2;
};

//モデルのキューブごとのデータの定義
struct DataPerCube
{
	float2 cubeUVArray[36];
	float4x4 cubeMatrix;
	uint objectDataIndex;
};

//モデルのキューブごとのデータ(StructuredBuffer)
StructuredBuffer<DataPerCube> DataPerCubeBuffer : register(t1);

//オブジェクトごとのデータの定義
struct DataPerObject
{
	float4x4 objectMatrix;
};

//オブジェクトごとのデータ(StructuredBuffer)
StructuredBuffer<DataPerObject> DataPerObjectBuffer : register(t2);

VS_OUT main(VS_IN input)
{
	//キューブごとのデータ
	DataPerCube dataPerCube = DataPerCubeBuffer[input.instanceID];

	VS_OUT output;
	//モデル用行列とオブジェクト行列を掛けてインスタンス用行列を計算
	float4x4 worldMat = mul(dataPerCube.cubeMatrix, DataPerObjectBuffer[dataPerCube.objectDataIndex].objectMatrix);
	//インスタンス用行列 * view * proj
	float4x4 mat = mul(mul(worldMat, view), projection);
	output.pos = mul(input.pos, mat);
	output.normal = normalize(mul(input.normal, (float3x3)worldMat));

	//UVをStructuredBufferから取り出す
	output.uv = dataPerCube.cubeUVArray[input.vertexID];

	output.worldPos = mul(input.pos, worldMat).xyz;
	output.color = float4(1.0, 1.0, 1.0, 1.0);

	return output;
}