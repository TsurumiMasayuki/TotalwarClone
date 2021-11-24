#include "TestScene.h"
#include <Actor\Base\GameObject.h>
#include <Device\GameDevice.h>
#include <Component\Graphics\Camera.h>
#include <Component\Graphics\SpriteRenderer.h>
#include <Component\Graphics\CircleRenderer.h>
#include <Component\Graphics\MeshRenderer.h>
#include <Component\Graphics\ParticleRenderer.h>
#include <Component\Graphics\PostEffectRenderer.h>
#include <Component\Graphics\InstancedRenderer.h>
#include <Component\Utility\Action\Actions.h>
#include <Component\Utility\Action\ActionManager.h>
#include <Component\Physics\BoxColliderBt.h>
#include <Component\Physics\SphereColliderBt.h>
#include <Component\Audio\AudioSource.h>
#include <Component\Utility\Action\Actions.h>
#include <Component\Utility\Action\ActionManager.h>
#include <Graphics\DX12\DX12GraphicsCore.h>
#include <Utility\Random.h>
#include <Utility\ModelGameObjectHelper.h>
#include <Math\Easing.h>
#include <Math\Plane.h>
#include <Math\MathUtility.h>

#include "Component\Graphics\D2DTextRenderer.h"

std::string TestScene::nextScene()
{
	return std::string();
}

bool TestScene::isEnd()
{
	return false;
}

void TestScene::start()
{
	m_pDefaultCamera->setDrawMode(DrawMode::DrawMode_3D);
	m_pDefaultCamera->getTransform().setLocalPosition(Vec3(0.0f, 0.0f, -10.0f));

	{
		GameObject* pObj = new GameObject(this);
		pObj->getTransform().setLocalPosition(Vec3(100.0f, 100.0f));
		pObj->getTransform().setLocalScale(Vec3(1.0f));

		auto pRenderer = pObj->addComponent<D2DTextRenderer>();
		pRenderer->setFont(L"‚l‚r ‚o–¾’©", L"ja-jp", 60);
		pRenderer->setText(L"ƒnƒ[");
		pRenderer->setTextRect(0.0f, 0.0f, 1280.0f, 720.0f);
		pRenderer->setColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
		pRenderer->setFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_MEDIUM);
		pRenderer->setFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
		pRenderer->setFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
		pRenderer->setTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
		pRenderer->setParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}
}

void TestScene::update()
{
}

void TestScene::shutdown()
{
}