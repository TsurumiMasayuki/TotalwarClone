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
	rasterizerState.CullMode = D3D12_CULL_MODE_BACK;	//�J�����O�ݒ�
	rasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	//�|���S�����h��Ԃ�
	rasterizerState.DepthClipEnable = true;			//�[�x�N���b�s���O��L����
	return rasterizerState;
}

void ValueMapMaterial::setMaterial(ID3D12GraphicsCommandList* pCommandList)
{
	if (g_pRootSignature.Get() == nullptr ||
		g_pPipelineState.Get() == nullptr) return;

	pCommandList->SetGraphicsRootSignature(g_pRootSignature.Get());
	pCommandList->SetPipelineState(g_pPipelineState.Get());

	//�f�X�N���v�^�q�[�v���Z�b�g
	pCommandList->SetDescriptorHeaps(1, m_pDescriptorHeap.GetAddressOf());

	//�萔�o�b�t�@���Z�b�g
	pCommandList->SetGraphicsRootConstantBufferView(0, m_pDefaultConstantBuffer->getGPUVirtualAddress());

	//�V�F�[�_�[���\�[�X�r���[
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
	//�f�X�N���v�^�q�[�v�̍쐬
	//�ݒ�\����
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.NumDescriptors = 2;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	//�V�F�[�_�[���猩����
	HRESULT result = pDevice->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(m_pDescriptorHeap.GetAddressOf()));

	auto handle = m_pDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	auto increment = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//�s��p�萔�o�b�t�@�̏�����
	m_pDefaultConstantBuffer->init(pDevice, handle);

	//�A�h���X��i�߂�
	handle.ptr += increment;

	//SRV�쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D12_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = m_pMainTexture->getFormat();
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	pDevice->CreateShaderResourceView(m_pMainTexture->getResource().Get(),
		&srvDesc,
		handle);

	//SRV�̃n���h�����v�Z
	auto gpuHandle = m_pDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	gpuHandle.ptr += increment;
	m_MainTextureSRV = gpuHandle;
}

D3D12_RENDER_TARGET_BLEND_DESC ValueMapMaterial::createBlendDesc()
{
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	ZeroMemory(&blendDesc, sizeof(D3D12_RENDER_TARGET_BLEND_DESC));
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	//RGBA�S�Ẵ`�����l����`��
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

	//SRV�쐬
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

	//���̓��C�A�E�g�擾
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputElements;
	int inputElementCount;
	m_pVertexShader->getInputLayout(inputElements, inputElementCount);

	D3D12_INPUT_ELEMENT_DESC* inputElementArray = new D3D12_INPUT_ELEMENT_DESC[inputElementCount];
	for (int i = 0; i < inputElementCount; i++)
		inputElementArray[i] = inputElements[i];

	//�V�F�[�_�[���擾
	std::vector<D3D12_ROOT_PARAMETER> rootParams;

	//���_�V�F�[�_�[���擾
	int vsRootParamCount;
	m_pVertexShader->getRootParams(rootParams, vsRootParamCount);

	//�W�I���g���V�F�[�_�[���擾
	int gsRootParamCount = 0;
	if (m_pGeometryShader != nullptr)
		m_pGeometryShader->getRootParams(rootParams, gsRootParamCount);

	//�s�N�Z���V�F�[�_�[���擾
	int psRootParamCount;
	m_pPixelShader->getRootParams(rootParams, psRootParamCount);

	int rootParamCount = vsRootParamCount + gsRootParamCount + psRootParamCount;
	D3D12_ROOT_PARAMETER* rootParamArray = new D3D12_ROOT_PARAMETER[rootParamCount];
	for (int i = 0; i < rootParamCount; i++)
		rootParamArray[i] = rootParams[i];

	//���_�V�F�[�_�[�f�[�^�擾
	unsigned char* pVSByteCode = nullptr;
	size_t vsByteCodeLength;
	m_pVertexShader->getShaderByteCode(&pVSByteCode, vsByteCodeLength);

	//�s�N�Z���V�F�[�_�[�f�[�^�擾
	unsigned char* pPSByteCode = nullptr;
	size_t psByteCodeLength;
	m_pPixelShader->getShaderByteCode(&pPSByteCode, psByteCodeLength);

	//�p�C�v���C���̐ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
	gPipeline.VS.pShaderBytecode = pVSByteCode;
	gPipeline.VS.BytecodeLength = vsByteCodeLength;

	//�W�I���g���V�F�[�_�[�f�[�^�擾
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

	gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;			//�W���ݒ�
	gPipeline.RasterizerState = createRasterizerState();

	gPipeline.InputLayout.pInputElementDescs = inputElementArray;
	gPipeline.InputLayout.NumElements = inputElementCount;
	gPipeline.PrimitiveTopologyType = m_PTType;

	gPipeline.NumRenderTargets = renderTargetCount;		//�����_�[�^�[�Q�b�g�̐�
	gPipeline.SampleDesc.Count = 1;		//�T���v����

	auto blendDesc = createBlendDesc();
	for (int i = 0; i < renderTargetCount; ++i)
	{
		//0~255�w���RGBA
		gPipeline.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM;
		//�u�����h�ݒ�
		gPipeline.BlendState.RenderTarget[i] = blendDesc;
		//RGBA�S�Ẵ`�����l����`��
		gPipeline.BlendState.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	}

	gPipeline.BlendState.AlphaToCoverageEnable = true;

	//�T���v���[�ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc = createSampler();

	//���[�g�V�O�l�`���ݒ�
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
		//�G���[���e���R�s�[
		std::string errStr;
		errStr.resize(pErrorBlob->GetBufferSize());

		std::copy_n(
			(char*)pErrorBlob->GetBufferPointer(),
			pErrorBlob->GetBufferSize(),
			errStr.begin());
		errStr += "\n";
		//�G���[���e���o��
		OutputDebugStringA(errStr.c_str());
		exit(1);
	}

	//���[�g�V�O�l�`�����쐬
	result = pDevice->CreateRootSignature(
		0, pRootSigBlob->GetBufferPointer(),
		pRootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(pRootSignature.GetAddressOf()));
	pRootSigBlob->Release();

	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gPipeline.pRootSignature = pRootSignature.Get();

	//�p�C�v���C�����쐬
	result = pDevice->CreateGraphicsPipelineState(&gPipeline, IID_PPV_ARGS(pPipeline.GetAddressOf()));

	delete[] inputElementArray;
	delete[] rootParamArray;
}
