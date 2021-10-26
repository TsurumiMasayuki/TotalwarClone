#pragma once
#include "Graphics\DX12\Material\AbstractMaterial.h"

class DX12Texture2D;

//Blockbenchモデル用マテリアル
class BBModelMaterial
	: public AbstractMaterial
{
public:
	BBModelMaterial();

	virtual D3D12_RASTERIZER_DESC createRasterizerState() override;
	virtual void setMaterial(ID3D12GraphicsCommandList* pCommandList) override;
	virtual void initShaders() override;
	virtual void initPipelineState(ID3D12Device* pDevice) override;
	virtual void initDescriptorHeap(ID3D12Device* pDevice) override;

	void setMainTexture(DX12Texture2D* pMainTexture);

private:
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> g_pRootSignature;
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> g_pPipelineState;

	DX12Texture2D* m_pMainTexture;
	D3D12_GPU_DESCRIPTOR_HANDLE m_MainTextureSRV;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pDescriptorHeap;
};