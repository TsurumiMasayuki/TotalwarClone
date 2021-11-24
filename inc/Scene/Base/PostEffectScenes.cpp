#include "PostEffectScenes.h"
#include <Actor\Base\GameObject.h>
#include <Device\GameDevice.h>
#include <Component\Graphics\Camera.h>
#include <Component\Graphics\SpriteRenderer.h>
#include <Component\Graphics\CircleRenderer.h>
#include <Component\Graphics\MeshRenderer.h>
#include <Component\Graphics\ParticleRenderer.h>
#include <Component\Graphics\PostEffectRenderer.h>
#include <Component\Graphics\GUI\GUISpriteRenderer.h>
#include <Component\Utility\Action\Actions.h>
#include <Component\Utility\Action\ActionManager.h>
#include <Component\Physics\BoxColliderBt.h>
#include <Component\Physics\SphereColliderBt.h>
#include <Component\Audio\AudioSource.h>
#include <Component\Utility\Action\Actions.h>
#include <Component\Utility\Action\ActionManager.h>
#include <Graphics\DX12\DX12GraphicsCore.h>
#include <Graphics\DX12\Material\PostEffectMaterials.h>
#include <Utility\Random.h>
#include <Utility\ModelGameObjectHelper.h>
#include <Math\Easing.h>
#include <Math\Plane.h>
#include <Math\MathUtility.h>

#include <fstream>

using namespace Action;

std::string GaussianBlur::nextScene()
{
	return "Bloom";
}

bool GaussianBlur::isEnd()
{
	return GameDevice::getInput().isKeyDown(DIK_SPACE);
}

void GaussianBlur::start()
{
	m_pDefaultCamera->setDrawMode(DrawMode::DrawMode_3D);
	m_pDefaultCamera->getTransform().setLocalPosition(Vec3(0.0f, 0.0f, 0.0f));

	//�}�e���A������
	m_pGaussianBlurMaterial = new GaussianBlurMaterial();
	m_pGaussianBlurMaterial->init(DX12GraphicsCore::g_pDevice.Get());

	//�����_�[�^�[�Q�b�g�̏�����
	float clearColor[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
	m_RenderTarget.init(DX12GraphicsCore::g_pDevice.Get(), 1280, 720, clearColor, 1);
	m_HBlurRenderTarget.init(DX12GraphicsCore::g_pDevice.Get(), 1280, 720, clearColor, 1);

	{
		auto pObj = ModelGameObjectHelper::instantiateModel<int>(this, GameDevice::getModelManager().getModel("boneTest"));
		pObj->getTransform().setLocalPosition(Vec3(0.0f, 0.0f, 10.0f));
		pObj->getTransform().setLocalScale(Vec3(1.0f));
		pObj->getTransform().setLocalAngles(Vec3(0.0f, 90.0f, 0.0f));
		pObj->getChildren().at(0)->getComponent<MeshRenderer>()->playAnimation();
	}

	{
		auto pObj = new GameObject(this);
		pObj->setParent(&m_pDefaultCamera->getUser());
		pObj->getTransform().setLocalPosition(Vec3(0.0f, 0.0f, 1.0f));
		pObj->getTransform().setLocalScale(Vec3(1280.0f, 720.0f, 1.0f));
		m_pPostEffectRenderer = pObj->addComponent<PostEffectRenderer>();
		m_pPostEffectRenderer->setMaterial(m_pGaussianBlurMaterial);
	}
}

void GaussianBlur::update()
{
}

void GaussianBlur::draw()
{
	auto& renderManager = GameDevice::getRenderManager();

	//�ʏ�`��
	m_pPostEffectRenderer->setActive(false);
	renderManager.setRenderTarget(&m_RenderTarget);
	renderManager.draw();
	m_pPostEffectRenderer->setActive(true);

	//�ʏ�`��̌��ʂ��Z�b�g
	m_pGaussianBlurMaterial->setTexture(&m_RenderTarget.getTexture(0));

	//��������1�s�N�Z�������炷
	float xOffset = 1.0f / 1280.0f;
	//�c������1�s�N�Z�������炷
	float yOffset = 1.0f / 720.0f;

	//�������̃I�t�Z�b�g�ݒ�
	m_pGaussianBlurMaterial->m_Offset = { xOffset, 0.0f, 0.0f, 0.0f };
	//�������u���[�p�����_�[�^�[�Q�b�g�ɕ`��
	renderManager.setRenderTarget(&m_HBlurRenderTarget);
	renderManager.drawSingle(m_pPostEffectRenderer);

	//�c�����̃I�t�Z�b�g�ݒ�
	m_pGaussianBlurMaterial->m_Offset = { 0.0f, yOffset, 0.0f, 0.0f };
	//�������u���[�p�����_�[�^�[�Q�b�g���Q�Ƃ��ĕ`��
	m_pGaussianBlurMaterial->setTexture(&m_HBlurRenderTarget.getTexture(0));
	renderManager.setDefaultRenderTarget();
	renderManager.drawSingle(m_pPostEffectRenderer);

	renderManager.present();
}

void GaussianBlur::shutdown()
{
	delete m_pGaussianBlurMaterial;
}

std::string Bloom::nextScene()
{
	return "GaussianBlur";
}

bool Bloom::isEnd()
{
	return GameDevice::getInput().isKeyDown(DIK_SPACE);
}

void Bloom::start()
{
	m_pDefaultCamera->setDrawMode(DrawMode::DrawMode_3D);
	m_pDefaultCamera->getTransform().setLocalPosition(Vec3(0.0f, 0.0f, 0.0f));

	//�}�e���A������
	m_pLuminanceExtractMaterial = new LuminanceExtractMaterial();
	m_pLuminanceExtractMaterial->init(DX12GraphicsCore::g_pDevice.Get());

	m_pGaussianBlurMaterial = new GaussianBlurMaterial();
	m_pGaussianBlurMaterial->init(DX12GraphicsCore::g_pDevice.Get());

	m_pAdditiveMaterial = new AdditiveMaterial();
	m_pAdditiveMaterial->init(DX12GraphicsCore::g_pDevice.Get());

	//�����_�[�^�[�Q�b�g�̏�����
	float clearColor[4] = { 0.0f, 191.0f / 255.0f, 1.0f, 1.0f };
	m_RenderTarget.init(DX12GraphicsCore::g_pDevice.Get(), 1280, 720, clearColor, 1);
	m_LuminanceRenderTarget.init(DX12GraphicsCore::g_pDevice.Get(), 1280, 720, clearColor, 1);
	m_HBlurRenderTarget.init(DX12GraphicsCore::g_pDevice.Get(), 1280, 720, clearColor, 1);
	m_GBlurRenderTarget.init(DX12GraphicsCore::g_pDevice.Get(), 1280, 720, clearColor, 1);

	{
		auto pObj = ModelGameObjectHelper::instantiateModel<int>(this, GameDevice::getModelManager().getModel("boneTest"));
		pObj->getTransform().setLocalPosition(Vec3(0.0f, 0.0f, 10.0f));
		pObj->getTransform().setLocalScale(Vec3(1.0f));
		pObj->getTransform().setLocalAngles(Vec3(0.0f, 90.0f, 0.0f));
		pObj->getChildren().at(0)->getComponent<MeshRenderer>()->playAnimation();
	}

	{
		auto pObj = new GameObject(this);
		pObj->setParent(&m_pDefaultCamera->getUser());
		pObj->getTransform().setLocalPosition(Vec3(0.0f, 0.0f, 1.0f));
		pObj->getTransform().setLocalScale(Vec3(1280.0f, 720.0f, 1.0f));
		m_pPostEffectRenderer = pObj->addComponent<PostEffectRenderer>();
	}

	auto pDevice = DX12GraphicsCore::g_pDevice.Get();

	//SRV�p�f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = 2;
	pDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(m_pSRVHeap.GetAddressOf()));

	//�}�e���A���̃p�����[�^�ݒ�
	m_pAdditiveMaterial->setSRVDescriptorHeap(m_pSRVHeap.Get());

	//SRV�쐬
	{
		//CPU���̃A�h���X
		auto cpuHandle = m_pSRVHeap.Get()->GetCPUDescriptorHandleForHeapStart();
		//GPU���̃A�h���X
		auto gpuHandle = m_pSRVHeap.Get()->GetGPUDescriptorHandleForHeapStart();

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D12_SHADER_RESOURCE_VIEW_DESC));
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

		//�}�e���A���̃p�����[�^�ݒ�1
		m_pAdditiveMaterial->setTextureHandle1(gpuHandle);

		//SRV1�쐬
		auto pResource1 = m_GBlurRenderTarget.getResource(0);
		pDevice->CreateShaderResourceView(pResource1.Get(),
			&srvDesc,
			cpuHandle);

		//�n���h���̃I�t�Z�b�g�v�Z
		auto offset = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		cpuHandle.ptr += offset;
		gpuHandle.ptr += offset;

		//�}�e���A���̃p�����[�^�ݒ�2
		m_pAdditiveMaterial->setTextureHandle2(gpuHandle);

		//SRV2�쐬
		auto pResource2 = m_RenderTarget.getResource(0);
		pDevice->CreateShaderResourceView(pResource2.Get(),
			&srvDesc,
			cpuHandle);
	}
}

void Bloom::update()
{
}

void Bloom::draw()
{
	auto& renderManager = GameDevice::getRenderManager();

	m_pPostEffectRenderer->setActive(false);
	renderManager.setRenderTarget(&m_RenderTarget);
	renderManager.draw();
	m_pPostEffectRenderer->setActive(true);

	//�}�e���A���Ƀe�N�X�`���ݒ�
	m_pLuminanceExtractMaterial->setTexture(&m_RenderTarget.getTexture(0));
	//�����_���[�Ƀ}�e���A���ݒ�
	m_pPostEffectRenderer->setMaterial(m_pLuminanceExtractMaterial);

	//�P�x���o�p�����_�[�^�[�Q�b�g�ɕ`��
	renderManager.setRenderTarget(&m_LuminanceRenderTarget);
	renderManager.drawSingle(m_pPostEffectRenderer);

	//�}�e���A���Ƀe�N�X�`���ݒ�
	m_pGaussianBlurMaterial->setTexture(&m_LuminanceRenderTarget.getTexture(0));
	//�����_���[�Ƀ}�e���A���ݒ�
	m_pPostEffectRenderer->setMaterial(m_pGaussianBlurMaterial);

	//��������1�s�N�Z�������炷
	float xOffset = 1.0f / 1280.0f;
	//�c������1�s�N�Z�������炷
	float yOffset = 1.0f / 720.0f;

	//�������̃I�t�Z�b�g�ݒ�
	m_pGaussianBlurMaterial->m_Offset = { xOffset, 0.0f, 0.0f, 0.0f };
	//�������u���[�p�����_�[�^�[�Q�b�g�ɕ`��
	renderManager.setRenderTarget(&m_HBlurRenderTarget);
	renderManager.drawSingle(m_pPostEffectRenderer);

	//�c�����̃I�t�Z�b�g�ݒ�
	m_pGaussianBlurMaterial->m_Offset = { 0.0f, yOffset, 0.0f, 0.0f };
	//�������u���[�p�����_�[�^�[�Q�b�g���Q�Ƃ��ĕ`��
	m_pGaussianBlurMaterial->setTexture(&m_HBlurRenderTarget.getTexture(0));
	renderManager.setRenderTarget(&m_GBlurRenderTarget);
	renderManager.drawSingle(m_pPostEffectRenderer);

	//�}�e���A���ݒ�
	m_pPostEffectRenderer->setMaterial(m_pAdditiveMaterial);
	renderManager.setDefaultRenderTarget();
	renderManager.drawSingle(m_pPostEffectRenderer);

	renderManager.present();
}

void Bloom::shutdown()
{
	delete m_pLuminanceExtractMaterial;
	delete m_pGaussianBlurMaterial;
	delete m_pAdditiveMaterial;
}
