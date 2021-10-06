#pragma once
#include <memory>
#include <DirectXMath.h>
#include <Graphics\DX12\DX12ConstantBuffer.h>
#include <Graphics\DX12\Material\AbstractMaterial.h>
#include <Utility\Color.h>

class DX12Texture2D;
class Transform;

class MeshMaterial
	: public AbstractMaterial
{
public:
	//���b�V���p�萔�o�b�t�@
	struct MeshBuffer
	{
		//�A���x�h
		DirectX::XMFLOAT4 baseColor;
		//�����x
		float metalness;
		//���ʔ��ˋ��x
		float specular;
		//�e��
		float roughness;

		float pad1;

		DirectX::XMFLOAT4 lightDir[3];
		DirectX::XMFLOAT4 lightColor[3];
		DirectX::XMFLOAT4 cameraPos;
	};

	//�X�L�j���O�p�萔�o�b�t�@
	struct SkinDataBuffer
	{
		DirectX::XMMATRIX bones[32];
	};

public:
	MeshMaterial();

	virtual D3D12_RASTERIZER_DESC createRasterizerState() override;
	virtual void setMaterial(ID3D12GraphicsCommandList* pCommandList) override;

	void setMainTexture(DX12Texture2D* pTexture);
	void setCameraTransform(Transform* pCameraTransform);

protected:
	virtual void initShaders() override;
	virtual void initPipelineState(ID3D12Device* pDevice) override;
	virtual void initDescriptorHeap(ID3D12Device* pDevice) override;

public:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pDescriptorHeap;
	std::unique_ptr<DX12ConstantBuffer<MeshBuffer>> m_pMeshConstantBuffer;
	std::unique_ptr<DX12ConstantBuffer<SkinDataBuffer>> m_pSkinDataBuffer;

	static Microsoft::WRL::ComPtr<ID3D12RootSignature> g_pRootSignature;
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> g_pPipelineState;

	Color m_BaseColor;
	float m_Metalness;
	float m_Specular;
	float m_Roughness;

private:
	DX12Texture2D* m_pMainTexture;
	Transform* m_pCameraTransform;
	D3D12_GPU_DESCRIPTOR_HANDLE m_MainTextureSRV;
};