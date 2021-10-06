#pragma once
#include <memory>
#include <DirectXMath.h>
#include <Graphics\DX12\DX12ConstantBuffer.h>
#include <Graphics\DX12\Material\AbstractMaterial.h>

class DX12Texture2D;

class CircleMaterial
	: public AbstractMaterial
{
public:
	//定数バッファ
	struct CircleBuffer
	{
		DirectX::XMFLOAT4 color;

		struct CircleParams
		{
			float step;
			float cutout;

		private:
			float dummy1;
			float dummy2;
		};

		CircleParams params;
	};

public:
	CircleMaterial();

	virtual D3D12_RASTERIZER_DESC createRasterizerState() override;
	virtual void setMaterial(ID3D12GraphicsCommandList* pCommandList) override;

protected:
	virtual void initShaders() override;
	virtual void initPipelineState(ID3D12Device * pDevice) override;
	virtual void initDescriptorHeap(ID3D12Device * pDevice) override;

public:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pDescriptorHeap;
	std::unique_ptr<DX12ConstantBuffer<CircleBuffer>> m_pConstantBuffer;

	static Microsoft::WRL::ComPtr<ID3D12RootSignature> g_pRootSignature;
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> g_pPipelineState;
};