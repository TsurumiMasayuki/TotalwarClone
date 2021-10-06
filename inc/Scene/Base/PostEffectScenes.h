#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <Graphics\DX12\DX12RenderTarget.h>
#include <Scene\Base\AbstractScene.h>

class PostEffectRenderer;
class GaussianBlurMaterial;
class LuminanceExtractMaterial;
class AdditiveMaterial;

class GaussianBlur
	: public AbstractScene
{
public:
	virtual std::string nextScene() override;
	virtual bool isEnd() override;

	virtual void start() override;
	virtual void update() override;
	virtual void draw() override;
	virtual void shutdown() override;

private:
	DX12RenderTarget m_RenderTarget;
	DX12RenderTarget m_HBlurRenderTarget;
	PostEffectRenderer* m_pPostEffectRenderer;
	GaussianBlurMaterial* m_pGaussianBlurMaterial;
};

class Bloom
	: public AbstractScene
{
public:
	virtual std::string nextScene() override;
	virtual bool isEnd() override;

	virtual void start() override;
	virtual void update() override;
	virtual void draw() override;
	virtual void shutdown() override;

private:
	DX12RenderTarget m_RenderTarget;
	DX12RenderTarget m_LuminanceRenderTarget;
	DX12RenderTarget m_HBlurRenderTarget;
	DX12RenderTarget m_GBlurRenderTarget;
	PostEffectRenderer* m_pPostEffectRenderer;

	LuminanceExtractMaterial* m_pLuminanceExtractMaterial;
	GaussianBlurMaterial* m_pGaussianBlurMaterial;
	AdditiveMaterial* m_pAdditiveMaterial;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pSRVHeap;
};