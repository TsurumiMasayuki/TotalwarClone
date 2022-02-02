#pragma once
#include <wrl.h>
#include <d3d12.h>

#include "Graphics\DX12\DX12RenderTarget.h"

class PostEffectRenderer;
class GaussianBlurMaterial;
class LuminanceExtractMaterial;
class AdditiveMaterial;

//�|�X�g�G�t�F�N�g�N���X(�u���[��)
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

	//�P�x���o�}�e���A��
	LuminanceExtractMaterial* m_pLuminanceExtractMaterial;
	//�K�E�V�A���u���[�}�e���A��
	GaussianBlurMaterial* m_pGaussianBlurMaterial;
	//�����}�e���A��
	AdditiveMaterial* m_pAdditiveMaterial;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pSRVHeap;
};