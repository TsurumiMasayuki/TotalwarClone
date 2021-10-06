#pragma once
#include <memory>
#include <DirectXMath.h>
#include <Graphics\DX12\DX12ConstantBuffer.h>
#include <Graphics\DX12\Material\AbstractMaterial.h>
#include <Utility\Color.h>

class DX12Texture2D;

class ParticleMaterial
	: public AbstractMaterial
{
public:
	//色用定数バッファ
	struct ColorBuffer
	{
		DirectX::XMFLOAT4 color;
	};

public:
	ParticleMaterial();

	virtual D3D12_RASTERIZER_DESC createRasterizerState() override;
	virtual void setMaterial(ID3D12GraphicsCommandList* pCommandList) override;

protected:
	virtual void initShaders() override;
	virtual void initPipelineState(ID3D12Device* pDevice) override;
	virtual void initDescriptorHeap(ID3D12Device* pDevice) override;

public:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pDescriptorHeap;
	std::unique_ptr<DX12ConstantBuffer<ColorBuffer>> m_pColorConstantBuffer;

	static Microsoft::WRL::ComPtr<ID3D12RootSignature> g_pRootSignature;
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> g_pPipelineState;
};

class InstancingMaterial
	: public AbstractMaterial
{
public:
	//メッシュ用定数バッファ
	struct MeshBuffer
	{
		DirectX::XMFLOAT3 lightDir;
	};

	InstancingMaterial();

	virtual D3D12_RASTERIZER_DESC createRasterizerState() override;
	virtual void setMaterial(ID3D12GraphicsCommandList* pCommandList) override;
	virtual void initShaders() override;
	virtual void initPipelineState(ID3D12Device* pDevice) override;
	virtual void initDescriptorHeap(ID3D12Device* pDevice) override;

	void setMainTexture(DX12Texture2D* pMainTexture);

public:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pDescriptorHeap;

private:
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> g_pRootSignature;
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> g_pPipelineState;

	DX12Texture2D* m_pMainTexture;
	D3D12_GPU_DESCRIPTOR_HANDLE m_MainTextureSRV;
};