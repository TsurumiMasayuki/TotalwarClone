#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <Device\Resource\DX12Mesh.h>

namespace fbxsdk
{
	class FbxScene;
}

struct Node
{
	~Node() { if (m_pMesh) delete m_pMesh; }

	//名前
	std::string m_Name;
	//ローカルスケール
	DirectX::XMVECTOR m_Scaling = { 1, 1, 1, 0 };
	//ローカル回転角
	DirectX::XMVECTOR m_Rotation = { 0, 0, 0, 0 };
	//ローカル回転角
	DirectX::XMVECTOR m_Translation = { 0, 0, 0, 0 };
	//ローカル変形行列
	DirectX::XMMATRIX m_Transform;
	//グローバル変換行列
	DirectX::XMMATRIX m_GlobalTransform;

	//メッシュ
	DX12Mesh* m_pMesh = nullptr;

	//親ノード
	Node* m_pParent = nullptr;

	std::vector<Node> m_Children;
};

class Model
{
public:
	friend class FbxLoader;
	friend class ModelGameObjectHelper;

public:
	~Model();

	fbxsdk::FbxScene* getFbxScene() { return m_pFbxScene; }

private:
	//名前
	std::string m_Name;
	//ルートノード
	Node m_Root;

	//アンビエント係数
	DirectX::XMFLOAT3 m_Ambient = { 1, 1, 1 };
	//ディフューズ係数
	DirectX::XMFLOAT3 m_Diffuse = { 1, 1, 1 };

	std::string m_FolderPath;

	fbxsdk::FbxScene* m_pFbxScene;

	DirectX::XMFLOAT3 m_BaseColor;
	float m_Metalness;
	float m_Specular;
	float m_Roughness;
};