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
	pCommandList->SetGraphicsRootDescriptorTable(1, m_MainTextureSRV);
}

void ValueMapMaterial::initShaders()
{
	m_pVertexShader = GameDevice::getShaderManager().getVertexShader<InstancingVertexShader>();
	m_pPixelShader = GameDevice::getShaderManager().getPixelShader<InstancingPixelShader>();
}

void ValueMapMaterial::initPipelineState(ID3D12Device* pDevice)
{
	if (g_pPipelineState.Get() == nullptr)
		createPipeline(pDevice, g_pRootSignature, g_pPipelineState);
}

void ValueMapMaterial::initDescriptorHeap(ID3D12Device* pDevice)
{
	//デスクリプタヒープの作成
	//設定構造体
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.NumDescriptors = 2;
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

	//SRVのハンドルを計算
	auto gpuHandle = m_pDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	gpuHandle.ptr += increment;
	m_MainTextureSRV = gpuHandle;
}

D3D12_RENDER_TARGET_BLEND_DESC ValueMapMaterial::createBlendDesc()
{
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	ZeroMemory(&blendDesc, sizeof(D3D12_RENDER_TARGET_BLEND_DESC));
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	//RGBA全てのチャンネルを描画
	blendDesc.BlendEnable = true;
	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	return blendDesc;
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

void ValueMapMaterial::createPipeline(ID3D12Device* pDevice, Microsoft::WRL::ComPtr<ID3D12RootSignature>& pRootSignature, Microsoft::WRL::ComPtr<ID3D12PipelineState>& pPipeline)
{
	HRESULT result;
	int renderTargetCount = 1;

	//入力レイアウト取得
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputElements;
	int inputElementCount;
	m_pVertexShader->getInputLayout(inputElements, inputElementCount);

	D3D12_INPUT_ELEMENT_DESC* inputElementArray = new D3D12_INPUT_ELEMENT_DESC[inputElementCount];
	for (int i = 0; i < inputElementCount; i++)
		inputElementArray[i] = inputElements[i];

	//シェーダー情報取得
	std::vector<D3D12_ROOT_PARAMETER> rootParams;

	//頂点シェーダー情報取得
	int vsRootParamCount;
	m_pVertexShader->getRootParams(rootParams, vsRootParamCount);

	//ジオメトリシェーダー情報取得
	int gsRootParamCount = 0;
	if (m_pGeometryShader != nullptr)
		m_pGeometryShader->getRootParams(rootParams, gsRootParamCount);

	//ピクセルシェーダー情報取得
	int psRootParamCount;
	m_pPixelShader->getRootParams(rootParams, psRootParamCount);

	int rootParamCount = vsRootParamCount + gsRootParamCount + psRootParamCount;
	D3D12_ROOT_PARAMETER* rootParamArray = new D3D12_ROOT_PARAMETER[rootParamCount];
	for (int i = 0; i < rootParamCount; i++)
		rootParamArray[i] = rootParams[i];

	//頂点シェーダーデータ取得
	unsigned char* pVSByteCode = nullptr;
	size_t vsByteCodeLength;
	m_pVertexShader->getShaderByteCode(&pVSByteCode, vsByteCodeLength);

	//ピクセルシェーダーデータ取得
	unsigned char* pPSByteCode = nullptr;
	size_t psByteCodeLength;
	m_pPixelShader->getShaderByteCode(&pPSByteCode, psByteCodeLength);

	//パイプラインの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
	gPipeline.VS.pShaderBytecode = pVSByteCode;
	gPipeline.VS.BytecodeLength = vsByteCodeLength;

	//ジオメトリシェーダーデータ取得
	if (m_pGeometryShader != nullptr)
	{
		unsigned char* pGSByteCode = nullptr;
		size_t gsByteCodeLength;
		m_pGeometryShader->getShaderByteCode(&pGSByteCode, gsByteCodeLength);

		gPipeline.GS.pShaderBytecode = pGSByteCode;
		gPipeline.GS.BytecodeLength = gsByteCodeLength;
	}

	gPipeline.PS.pShaderBytecode = pPSByteCode;
	gPipeline.PS.BytecodeLength = psByteCodeLength;

	gPipeline.DepthStencilState.DepthEnable = true;
	gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;			//標準設定
	gPipeline.RasterizerState = createRasterizerState();

	gPipeline.InputLayout.pInputElementDescs = inputElementArray;
	gPipeline.InputLayout.NumElements = inputElementCount;
	gPipeline.PrimitiveTopologyType = m_PTType;

	gPipeline.NumRenderTargets = renderTargetCount;		//レンダーターゲットの数
	gPipeline.SampleDesc.Count = 1;		//サンプル回数

	auto blendDesc = createBlendDesc();
	for (int i = 0; i < renderTargetCount; ++i)
	{
		//0~255指定のRGBA
		gPipeline.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM;
		//ブレンド設定
		gPipeline.BlendState.RenderTarget[i] = blendDesc;
		//RGBA全てのチャンネルを描画
		gPipeline.BlendState.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	}

	gPipeline.BlendState.AlphaToCoverageEnable = true;

	//サンプラー設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc = createSampler();

	//ルートシグネチャ設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParamArray;
	rootSignatureDesc.NumParameters = rootParamCount;
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	ID3DBlob* pRootSigBlob = nullptr;
	ID3DBlob* pErrorBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &pRootSigBlob, &pErrorBlob);

	if (FAILED(result))
	{
		//エラー内容をコピー
		std::string errStr;
		errStr.resize(pErrorBlob->GetBufferSize());

		std::copy_n(
			(char*)pErrorBlob->GetBufferPointer(),
			pErrorBlob->GetBufferSize(),
			errStr.begin());
		errStr += "\n";
		//エラー内容を出力
		OutputDebugStringA(errStr.c_str());
		exit(1);
	}

	//ルートシグネチャを作成
	result = pDevice->CreateRootSignature(
		0, pRootSigBlob->GetBufferPointer(),
		pRootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(pRootSignature.GetAddressOf()));
	pRootSigBlob->Release();

	//パイプラインにルートシグネチャをセット
	gPipeline.pRootSignature = pRootSignature.Get();

	//パイプラインを作成
	result = pDevice->CreateGraphicsPipelineState(&gPipeline, IID_PPV_ARGS(pPipeline.GetAddressOf()));

	delete[] inputElementArray;
	delete[] rootParamArray;
}
