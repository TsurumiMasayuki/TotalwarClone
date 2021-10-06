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

	//���O
	std::string m_Name;
	//���[�J���X�P�[��
	DirectX::XMVECTOR m_Scaling = { 1, 1, 1, 0 };
	//���[�J����]�p
	DirectX::XMVECTOR m_Rotation = { 0, 0, 0, 0 };
	//���[�J����]�p
	DirectX::XMVECTOR m_Translation = { 0, 0, 0, 0 };
	//���[�J���ό`�s��
	DirectX::XMMATRIX m_Transform;
	//�O���[�o���ϊ��s��
	DirectX::XMMATRIX m_GlobalTransform;

	//���b�V��
	DX12Mesh* m_pMesh = nullptr;

	//�e�m�[�h
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
	//���O
	std::string m_Name;
	//���[�g�m�[�h
	Node m_Root;

	//�A���r�G���g�W��
	DirectX::XMFLOAT3 m_Ambient = { 1, 1, 1 };
	//�f�B�t���[�Y�W��
	DirectX::XMFLOAT3 m_Diffuse = { 1, 1, 1 };

	std::string m_FolderPath;

	fbxsdk::FbxScene* m_pFbxScene;

	DirectX::XMFLOAT3 m_BaseColor;
	float m_Metalness;
	float m_Specular;
	float m_Roughness;
};