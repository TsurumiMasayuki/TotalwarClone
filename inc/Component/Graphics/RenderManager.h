#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#include <Utility\INonCopyable.h>
#include <Utility\ManagerHelper.h>

#include <Component\Graphics\Camera.h>
#include <Component\Graphics\IRenderer.h>

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

	//�����_�[�^�[�Q�b�g����ʕ`��p�ɕύX
	void setDefaultRenderTarget();
	//�����_�[�^�[�Q�b�g��o�^(���ɓo�^����Ă�����̂̓N���A�����)
	void setRenderTarget(DX12RenderTarget* pRenderTarget);

	//�o�^���ꂽ���̂�S�ĕ`��
	void draw();
	//�w�肳�ꂽRenderer�̂ݕ`��
	void drawSingle(IRenderer* pRenderer);
	//�E�B���h�E��ɕ\��
	void present();

private:
	ManagerHelper<Camera*> m_CameraList;
	ManagerHelper<IRenderer*> m_RendererList;

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator>		m_pCommandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>	m_pCommandList;
	Microsoft::WRL::ComPtr<ID3D12Fence>					m_pFence;
	UINT m_FenceValue;

	bool m_RTVFlag;

	DX12RenderTarget m_BackBuffer1;
	DX12RenderTarget m_BackBuffer2;

	DX12RenderTarget* m_pRenderTarget;
};