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

	//�����_�[�^�[�Q�b�g����ʕ`��p�ɕύX
	void setDefaultRenderTarget();
	//�����_�[�^�[�Q�b�g��o�^(���ɓo�^����Ă�����̂̓N���A�����)
	void setRenderTarget(DX12RenderTarget* pRenderTarget);

	//�o�^���ꂽ���̂�S�ĕ`��(D2D�n��`�悷�邩�I����)
	void draw(bool isDrawD2D = true);
	//�w�肳�ꂽRenderer�̂ݕ`��
	void drawSingle(IRenderer* pRenderer, bool isDrawD2D = true);
	//�E�B���h�E��ɕ\��
	void present();

private:
	//D2D�n�`��
	void drawD2D();

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
	DX12RenderTarget* m_pRenderTarget;
};