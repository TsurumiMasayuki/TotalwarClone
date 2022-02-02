#include "BloomPostEffect.h"

#include "Component\Graphics\PostEffectRenderer.h"
#include "Def\Screen.h"
#include "Device\GameDevice.h"
#include "Graphics\DX12\Material\PostEffectMaterials.h"

BloomPostEffect::BloomPostEffect(PostEffectRenderer* pPostEffectRenderer)
	: m_pPostEffectRenderer(pPostEffectRenderer),
	m_pLuminanceExtractMaterial(new LuminanceExtractMaterial()),
	m_pGaussianBlurMaterial(new GaussianBlurMaterial()),
	m_pAdditiveMaterial(new AdditiveMaterial())
{
}

BloomPostEffect::~BloomPostEffect()
{
	//マテリアルを削除
	delete m_pLuminanceExtractMaterial;
	delete m_pGaussianBlurMaterial;
	delete m_pAdditiveMaterial;
}

void BloomPostEffect::init()
{
	//マテリアルの初期化
	auto pDevice = DX12GraphicsCore::g_pDevice.Get();
	m_pLuminanceExtractMaterial->init(pDevice);
	m_pGaussianBlurMaterial->init(pDevice);
	m_pAdditiveMaterial->init(pDevice);

	int width = (int)WindowWidth;
	int height = (int)WindowHeight;

	//レンダーターゲットの初期化
	float clearColor[4] = { 0.0f / 256.0f, 111.0f / 256.0f, 134.0f / 256.0f, 1.0f };

	m_RenderTarget.init(pDevice, width, height, clearColor, 1);
	m_LuminanceRenderTarget.init(pDevice, width, height, clearColor, 1);
	m_HBlurRenderTarget.init(pDevice, width, height, clearColor, 1);
	m_GBlurRenderTarget.init(pDevice, width, height, clearColor, 1);

	//SRV用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = 2;
	pDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(m_pSRVHeap.GetAddressOf()));

	m_pAdditiveMaterial->setSRVDescriptorHeap(m_pSRVHeap.Get());

	//SRV作成
	{
		//CPU側のアドレス
		auto cpuHandle = m_pSRVHeap.Get()->GetCPUDescriptorHandleForHeapStart();
		//GPU側のアドレス
		auto gpuHandle = m_pSRVHeap.Get()->GetGPUDescriptorHandleForHeapStart();

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D12_SHADER_RESOURCE_VIEW_DESC));
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

		//マテリアルのパラメータ設定1
		m_pAdditiveMaterial->setTextureHandle1(gpuHandle);

		//SRV1作成
		auto pResource1 = m_GBlurRenderTarget.getResource(0);
		pDevice->CreateShaderResourceView(pResource1.Get(),
			&srvDesc,
			cpuHandle);

		//ハンドルのオフセット計算
		auto offset = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		cpuHandle.ptr += offset;
		gpuHandle.ptr += offset;

		//マテリアルのパラメータ設定2
		m_pAdditiveMaterial->setTextureHandle2(gpuHandle);

		//SRV2作成
		auto pResource2 = m_RenderTarget.getResource(0);
		pDevice->CreateShaderResourceView(pResource2.Get(),
			&srvDesc,
			cpuHandle);
	}
}

void BloomPostEffect::draw()
{
	auto& renderManager = GameDevice::getRenderManager();

	m_pPostEffectRenderer->setActive(false);
	renderManager.setRenderTarget(&m_RenderTarget);
	renderManager.draw(false);
	m_pPostEffectRenderer->setActive(true);

	//マテリアルにテクスチャ設定
	m_pLuminanceExtractMaterial->setTexture(&m_RenderTarget.getTexture(0));
	//レンダラーにマテリアル設定
	m_pPostEffectRenderer->setMaterial(m_pLuminanceExtractMaterial);

	//輝度抽出用レンダーターゲットに描画
	renderManager.setRenderTarget(&m_LuminanceRenderTarget);
	renderManager.drawSingle(m_pPostEffectRenderer, false);

	//マテリアルにテクスチャ設定
	m_pGaussianBlurMaterial->setTexture(&m_LuminanceRenderTarget.getTexture(0));
	//レンダラーにマテリアル設定
	m_pPostEffectRenderer->setMaterial(m_pGaussianBlurMaterial);

	//横方向に1ピクセル分ずらす
	float xOffset = 1.0f / WindowWidth;
	//縦方向に1ピクセル分ずらす
	float yOffset = 1.0f / WindowHeight;

	//横方向のオフセット設定
	m_pGaussianBlurMaterial->m_Offset = { xOffset, 0.0f, 0.0f, 0.0f };
	//横方向ブラー用レンダーターゲットに描画
	renderManager.setRenderTarget(&m_HBlurRenderTarget);
	renderManager.drawSingle(m_pPostEffectRenderer, false);

	//縦方向のオフセット設定
	m_pGaussianBlurMaterial->m_Offset = { 0.0f, yOffset, 0.0f, 0.0f };
	//横方向ブラー用レンダーターゲットを参照して描画
	m_pGaussianBlurMaterial->setTexture(&m_HBlurRenderTarget.getTexture(0));
	renderManager.setRenderTarget(&m_GBlurRenderTarget);
	renderManager.drawSingle(m_pPostEffectRenderer, false);

	//マテリアル設定
	m_pPostEffectRenderer->setMaterial(m_pAdditiveMaterial);
	renderManager.setDefaultRenderTarget();
	renderManager.drawSingle(m_pPostEffectRenderer);

	renderManager.present();
}
