#pragma once
#include <wrl.h>
#include <memory>
#include <d3d12.h>
#include <DirectXMath.h>
#include <Utility\INonCopyable.h>
#include <Graphics\DX12\DX12ConstantBuffer.h>
#include <Graphics\DX12\Shader\AbstractVertexShader.h>
#include <Graphics\DX12\Shader\AbstractGeometryShader.h>
#include <Graphics\DX12\Shader\AbstractPixelShader.h>

class AbstractMaterial
	: private INonCopyable
{
public:
	//ワールド行列等の情報用定数バッファ
	struct DefaultBuffer
	{
		DirectX::XMFLOAT4X4 wvp;
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 worldInv;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};

	AbstractMaterial();

	void init(ID3D12Device* pDevice);

	virtual D3D12_RASTERIZER_DESC createRasterizerState() = 0;
	virtual D3D12_RENDER_TARGET_BLEND_DESC createBlendDesc();
	virtual D3D12_STATIC_SAMPLER_DESC createSampler();
	virtual void setMaterial(ID3D12GraphicsCommandList* pCommandList) = 0;

protected:
	virtual void initShaders() = 0;
	virtual void initPipelineState(ID3D12Device* pDevice) = 0;
	virtual void initDescriptorHeap(ID3D12Device* pDevice) = 0;

	void createPipelineState(ID3D12Device* pDevice,
		Microsoft::WRL::ComPtr<ID3D12RootSignature>& pRootSignature,
		Microsoft::WRL::ComPtr<ID3D12PipelineState>& pPipeline);

public:
	std::unique_ptr<DX12ConstantBuffer<DefaultBuffer>> m_pDefaultConstantBuffer;

protected:
	AbstractVertexShader* m_pVertexShader;
	AbstractGeometryShader* m_pGeometryShader;
	AbstractPixelShader* m_pPixelShader;

	D3D12_PRIMITIVE_TOPOLOGY_TYPE m_PTType;
};
