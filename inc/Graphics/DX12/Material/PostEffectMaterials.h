#pragma once
#include <Graphics\DX12\DX12ConstantBuffer.h>
#include <Graphics\DX12\Material\AbstractMaterial.h>

class DX12Texture2D;

class GaussianBlurMaterial
	: public AbstractMaterial
{
public:
	static const int weightCount = 32;
	float m_Weights[weightCount];
	DirectX::XMFLOAT4 m_Offset;

private:
	struct GaussianBlurBuffer
	{
		//ブラーのオフセット
		DirectX::XMFLOAT4 offset;
		//重みが入った配列
		DirectX::XMFLOAT4 weights[weightCount];
	};

public:
	GaussianBlurMaterial();

	virtual D3D12_RASTERIZER_DESC createRasterizerState() override;
	virtual void setMaterial(ID3D12GraphicsCommandList* pCommandList) override;
	virtual void initShaders() override;
	virtual void initPipelineState(ID3D12Device* pDevice) override;
	virtual void initDescriptorHeap(ID3D12Device* pDevice) override;

	void setTexture(DX12Texture2D* pTexture2D);

public:
	std::unique_ptr<DX12ConstantBuffer<GaussianBlurBuffer>> m_pGaussianBuffer;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pDescriptorHeap;

	static Microsoft::WRL::ComPtr<ID3D12RootSignature> g_pRootSignature;
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> g_pPipelineState;

	DX12Texture2D* m_pTexture2D;
	D3D12_GPU_DESCRIPTOR_HANDLE m_TextureSRV;
};

class LuminanceExtractMaterial
	: public AbstractMaterial
{
public:
	LuminanceExtractMaterial();

	virtual D3D12_RASTERIZER_DESC createRasterizerState() override;
	virtual void setMaterial(ID3D12GraphicsCommandList* pCommandList) override;
	virtual void initShaders() override;
	virtual void initPipelineState(ID3D12Device* pDevice) override;
	virtual void initDescriptorHeap(ID3D12Device* pDevice) override;

	void setTexture(DX12Texture2D* pTexture2D);

private:
	DX12Texture2D* m_pTexture2D;
	D3D12_GPU_DESCRIPTOR_HANDLE m_TextureSRV;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pDescriptorHeap;

	static Microsoft::WRL::ComPtr<ID3D12RootSignature> g_pRootSignature;
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> g_pPipelineState;
};

class AdditiveMaterial
	: public AbstractMaterial
{
public:
	AdditiveMaterial();

	virtual D3D12_RASTERIZER_DESC createRasterizerState() override;
	virtual void setMaterial(ID3D12GraphicsCommandList* pCommandList) override;
	virtual void initShaders() override;
	virtual void initPipelineState(ID3D12Device* pDevice) override;
	virtual void initDescriptorHeap(ID3D12Device* pDevice) override;

	void setSRVDescriptorHeap(ID3D12DescriptorHeap* pSRVDescHeap);
	void setTextureHandle1(const D3D12_GPU_DESCRIPTOR_HANDLE& handle1);
	void setTextureHandle2(const D3D12_GPU_DESCRIPTOR_HANDLE& handle2);

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pDescriptorHeap;

	static Microsoft::WRL::ComPtr<ID3D12RootSignature> g_pRootSignature;
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> g_pPipelineState;

	ID3D12DescriptorHeap* m_pSRVDescHeap;
	D3D12_GPU_DESCRIPTOR_HANDLE m_Handle1;
	D3D12_GPU_DESCRIPTOR_HANDLE m_Handle2;
};