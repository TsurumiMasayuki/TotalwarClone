#pragma once
#include <wrl.h>
#include <d3d12.h>

#include "Graphics\DX12\DX12RenderTarget.h"

class PostEffectRenderer;
class GaussianBlurMaterial;
class LuminanceExtractMaterial;
class AdditiveMaterial;

//ポストエフェクトクラス(ブルーム)
class BloomPostEffect
{
public:
	BloomPostEffect(PostEffectRenderer* pPostEffectRenderer);
	~BloomPostEffect();

	void init();
	void draw();

private:
	DX12RenderTarget m_RenderTarget;
	DX12RenderTarget m_LuminanceRenderTarget;
	DX12RenderTarget m_HBlurRenderTarget;
	DX12RenderTarget m_GBlurRenderTarget;
	PostEffectRenderer* m_pPostEffectRenderer;

	//輝度抽出マテリアル
	LuminanceExtractMaterial* m_pLuminanceExtractMaterial;
	//ガウシアンブラーマテリアル
	GaussianBlurMaterial* m_pGaussianBlurMaterial;
	//合成マテリアル
	AdditiveMaterial* m_pAdditiveMaterial;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pSRVHeap;
};