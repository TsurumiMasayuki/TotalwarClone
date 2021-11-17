#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#include <Utility\INonCopyable.h>
#include <Utility\ManagerHelper.h>

#include <Component\Graphics\Camera.h>
#include <Component\Graphics\IRenderer.h>
#include <Component\Graphics\ID2DRenderer.h>

#include <Graphics\DX12\DX12GraphicsCore.h>
#include <Graphics\DX12\DX12RenderTarget.h>

class RenderManager
	: private INonCopyable
{
public:
	void init();
	void update();

	void addCamera(Camera* pCamera);
	void removeCamera(Camera* pCamera);

	void addRenderer(IRenderer* pRenderer);
	void removeRenderer(IRenderer* pRenderer);

	void addD2DRenderer(ID2DRenderer* pRenderer);
	void removeD2DRenderer(ID2DRenderer* pRenderer);

	//レンダーターゲットを画面描画用に変更
	void setDefaultRenderTarget();
	//レンダーターゲットを登録(既に登録されているものはクリアされる)
	void setRenderTarget(DX12RenderTarget* pRenderTarget);

	//登録されたものを全て描画
	void draw();
	//指定されたRendererのみ描画
	void drawSingle(IRenderer* pRenderer);
	//ウィンドウ上に表示
	void present();

private:
	ManagerHelper<Camera*> m_CameraList;
	ManagerHelper<IRenderer*> m_RendererList;
	ManagerHelper<ID2DRenderer*> m_D2DRendererList;

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_pCommandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_pCommandList;
	Microsoft::WRL::ComPtr<ID3D12Fence>	m_pFence;

	UINT m_FenceValue;

	bool m_RTVFlag;

	DX12RenderTarget m_BackBuffers[2];
	Microsoft::WRL::ComPtr<ID3D11Resource> m_WrappedBackBuffers[2];
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_D2DRenderTargets[2];

	DX12RenderTarget* m_pRenderTarget;
};