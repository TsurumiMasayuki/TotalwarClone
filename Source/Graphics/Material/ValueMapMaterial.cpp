#include "ValueMapMaterial.h"
#include "Device\GameDevice.h"
#include "Graphics\DX12\Shader\DefaultShaders.h"

Microsoft::WRL::ComPtr<ID3D12RootSignature> ValueMapMaterial::g_pRootSignature;
Microsoft::WRL::ComPtr<ID3D12PipelineState> ValueMapMaterial::g_pPipelineState;

ValueMapMaterial::ValueMapMaterial()
	: m_pMainTexture(GameDevice::getTextureManager().getTexture("BoxFill"))
{
}

D3D12_RASTERIZER_DESC ValueMapMaterial::createRasterizerState()
{
	D3D12_RASTERIZER_DESC rasterizerState;
	ZeroMemory(&rasterizerState, sizeof(rasterizerState));
	rasterizerState.CullMode = D3D12_CULL_MODE_BACK;	//カリング設定
	rasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	//ポリゴン内塗りつぶし
	rasterizerState.DepthClipEnable = true;			//深度クリッピングを有効に
	return rasterizerState;
}

void ValueMapMaterial::setMaterial(ID3D12GraphicsCommandList* pCommandList)
{
	if (g_pRootSignature.Get() == nullptr ||
		g_pPipelineState.Get() == nullptr) return;

	pCommandList->SetGraphicsRootSignature(g_pRootSignature.Get());
	pCommandList->SetPipelineState(g_pPipelineState.Get());

	//デスクリプタヒープをセット
	pCommandList->SetDescriptorHeaps(1, m_pDescriptorHeap.GetAddressOf());

	//定数バッファをセット
	pCommandList->SetGraphicsRootConstantBufferView(0, m_pDefaultConstantBuffer->getGPUVirtualAddress());

	//シェーダーリソースビュー
	pCommandList->SetGraphicsRootDescriptorTable(2, m_MainTextureSRV);
}

void ValueMapMaterial::initShaders()
{
	m_pVertexShader = GameDevice::getShaderManager().getVertexShader<InstancingVertexShader>();
	m_pPixelShader = GameDevice::getShaderManager().getPixelShader<InstancingPixelShader>();
}

void ValueMapMaterial::initPipelineState(ID3D12Device* pDevice)
{
}

void ValueMapMaterial::initDescriptorHeap(ID3D12Device* pDevice)
{
}

void ValueMapMaterial::setMainTexture(DX12Texture2D* pMainTexture)
{
	m_pMainTexture = pMainTexture;

	auto handle = m_pDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	auto increment = DX12GraphicsCore::g_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	handle.ptr += increment;

	//SRV作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D12_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = m_pMainTexture->getFormat();
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	DX12GraphicsCore::g_pDevice->CreateShaderResourceView(m_pMainTexture->getResource().Get(),
		&srvDesc,
		handle);
}
