#pragma once
#include <string>
#include <vector>
#include <d3d12.h>
#include <Graphics\DX12\DX12VertexBuffer.h>
#include <Graphics\DX12\DX12IndexBuffer.h>
#include <Utility\INonCopyable.h>

namespace fbxsdk
{
	class FbxMesh;
	class FbxCluster;
	class FbxScene;
}

struct Bone
{
	//名前
	std::string m_Name;
	//初期姿勢の逆行列
	DirectX::XMMATRIX m_InvInitialPose;
	//クラスター
	fbxsdk::FbxCluster* m_pFbxCluster;
	//コンストラクタ
	Bone(const std::string& name)
		: m_Name(name),
		m_pFbxCluster(nullptr)
	{}
};

//メッシュ用構造体
class DX12Mesh
	: private INonCopyable
{
public:
	static const int MAX_BONE_INDICES = 4;

	struct MeshVertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;

		UINT boneIndex[MAX_BONE_INDICES];
		float boneWeight[MAX_BONE_INDICES];
	};

public:
	DX12Mesh();
	~DX12Mesh();

	//初期化処理
	void init(ID3D12Device* pDevice, const std::vector<MeshVertex>& vertices, const std::vector<USHORT>& indices, const std::string& textureName);

	//頂点バッファの取得
	const DX12VertexBuffer<MeshVertex>& getVertexBuffer();
	//インデックスバッファの取得
	const DX12IndexBuffer& getIndexBuffer();

	//テクスチャ名の取得
	std::string getTextureName() { return m_TextureName; }

	int getVertexCount();
	const std::vector<MeshVertex>& getVertices() const;
	const std::vector<USHORT>& getIndices() const;

	std::vector<Bone>& getBones() { return m_Bones; }

public:
	//Fbxシーン
	fbxsdk::FbxScene* m_pFbxScene;

private:
	DX12VertexBuffer<MeshVertex> m_VertexBuffer;
	DX12IndexBuffer m_IndexBuffer;
	int m_VertexCount;
	std::string m_TextureName;

	std::vector<Bone> m_Bones;

	std::vector<MeshVertex> m_Vertices;
	std::vector<USHORT> m_Indices;
};