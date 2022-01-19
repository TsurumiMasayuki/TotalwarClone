#include "BBModelMaterial.h"
#include "Device\GameDevice.h"
#include "Graphics\DX12\DX12Texture2D.h"
#include "Graphics\DX12\Shader\DefaultShaders.h"
#include "Graphics\Shader\BBInstancingVertexShader.h"
#include "Device\Resource\TextureManager.h"

Microsoft::WRL::ComPtr<ID3D12RootSignature> BBModelMaterial::g_pRootSignature;
Microsoft::WRL::ComPtr<ID3D12PipelineState> BBModelMaterial::g_pPipelineState;

BBModelMaterial::BBModelMaterial()
	: m_pMainTexture(GameDevice::getTextureManager().getTexture("BoxFill"))
{
}

D3D12_RASTERIZER_DESC BBModelMaterial::createRasterizerState()
{
	D3D12_RASTERIZER_DESC rasterizerState;
	ZeroMemory(&rasterizerState, sizeof(rasterizerState));
	rasterizerState.CullMode = D3D12_CULL_MODE_BACK;	//カリング設定
	rasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	//ポリゴン内塗りつぶし
	rasterizerState.DepthClipEnable = true;			//深度クリッピングを有効に
	return rasterizerState;
}

void BBModelMaterial::setMaterial(ID3D12GraphicsCommandList* pCommandList)
{
	if (g_pRootSignature.Get() == nullptr ||
		g_pPipelineState.Get() == nullptr) return;

	pCommandList->SetGraphicsRootSignature(g_pRootSignature.Get());
	pCommandList->SetPipelineState(g_pPipelineState.Get());

	//デスクリプタヒープをセット
	pCommandList->SetDescriptorHeaps(1, m_pDescriptorHeap.GetAddressOf());

	//定数バッファをセット
	pCommandList->SetGraphicsRootConstantBufferView(0, m_pDefaultConstantBuffer->getGPUVirtualAddress());

	//モデル用StructuredBufferをセット
	pCommandList->SetGraphicsRootShaderResourceView(1, m_DataPerCubeBuffer.getGPUVirtualAddress());

	//オブジェクト用StructuredBufferをセット
	pCommandList->SetGraphicsRootShaderResourceView(2, m_DataPerObjectBuffer.getGPUVirtualAddress());

	//テクスチャをセット
	pCommandList->SetGraphicsRootDescriptorTable(3, m_MainTextureSRV);
}

void BBModelMaterial::initShaders()
{
	m_pVertexShader = GameDevice::getShaderManager().getVertexShader<BBInstancingVertexShader>();
	m_pPixelShader = GameDevice::getShaderManager().getPixelShader<InstancingPixelShader>();
}

void BBModelMaterial::initPipelineState(ID3D12Device* pDevice)
{
	if (g_pPipelineState.Get() == nullptr)
		createPipelineState(pDevice, g_pRootSignature, g_pPipelineState);
}

void BBModelMaterial::initDescriptorHeap(ID3D12Device* pDevice)
{
	// デスクリプタヒープの作成
	//設定構造体
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.NumDescriptors = 4;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	//シェーダーから見える
	HRESULT result = pDevice->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(m_pDescriptorHeap.GetAddressOf()));

	auto handle = m_pDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	auto increment = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//行列用定数バッファの初期化
	m_pDefaultConstantBuffer->init(pDevice, handle);

	//アドレスを進める
	handle.ptr += increment;

	//SRV作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D12_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = m_pMainTexture->getFormat();
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	pDevice->CreateShaderResourceView(m_pMainTexture->getResource().Get(),
		&srvDesc,
		handle);

	//テクスチャのSRVのハンドルを計算
	auto gpuHandle = m_pDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	gpuHandle.ptr += increment;
	m_MainTextureSRV = gpuHandle;
}

void BBModelMaterial::setBBModel(const BlockbenchModel& bbModel)
{
	m_pBBModel = &bbModel;
	//テクスチャ名をモデルから取得してセット
	m_pMainTexture = GameDevice::getTextureManager().getTexture(m_pBBModel->getTextureName());

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

void BBModelMaterial::setModelCount(int modelCount)
{
	m_ModelCount = modelCount;
}

void BBModelMaterial::initDataPerCube()
{
	auto handle = m_pDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	auto increment = DX12GraphicsCore::g_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	handle.ptr += increment * 2;

	//キューブ全体の数を計算
	int cubeDataCount = m_pBBModel->getCubeCount() * m_ModelCount;

	//キューブごとのデータを初期化
	m_DataPerCubeBuffer.init(DX12GraphicsCore::g_pDevice.Get(), handle, sizeof(DataPerCube) * cubeDataCount, cubeDataCount);

	//GPU上の仮想メモリを適用
	DataPerCube* pMap = nullptr;
	m_DataPerCubeBuffer.map(0, nullptr, &pMap);

	const auto& cubes = m_pBBModel->getCubes();
	int modelCubeCount = m_pBBModel->getCubeCount();

	for (int i = 0; i < cubeDataCount; i++)
	{
		const auto& cube = cubes.at(i % modelCubeCount);

		for (int j = 0; j < 36; j++)
		{
			pMap[i].m_UVArray[j].x = cube.m_UVArray[j].x;
			pMap[i].m_UVArray[j].y = cube.m_UVArray[j].y;
		}

		DirectX::XMStoreFloat4x4(&pMap[i].m_Matrix, DirectX::XMMatrixTranspose(cube.m_Matrix));

		//DataPerObjectに対応するインデックスを算出
		pMap[i].m_ObjectDataIndex = i / modelCubeCount;
	}

	m_DataPerCubeBuffer.unMap(0, nullptr);
}

void BBModelMaterial::initDataPerObject()
{
	auto handle = m_pDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	auto increment = DX12GraphicsCore::g_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	handle.ptr += increment * 3;

	//オブジェクトごとのデータを初期化
	m_DataPerObjectBuffer.init(DX12GraphicsCore::g_pDevice.Get(), handle, sizeof(DataPerObject) * m_ModelCount, m_ModelCount);
}

void BBModelMaterial::updateDataPerObject(const std::vector<DirectX::XMMATRIX>& dataPerObject)
{
	//GPU上の仮想メモリを適用
	DataPerObject* pMap = nullptr;

	m_DataPerObjectBuffer.map(0, nullptr, &pMap);

	for (int i = 0; i < m_ModelCount; i++)
	{
		DirectX::XMStoreFloat4x4(&pMap[i].m_Matrix, DirectX::XMMatrixTranspose(dataPerObject.at(i)));
	}

	m_DataPerObjectBuffer.unMap(0, nullptr);
}
