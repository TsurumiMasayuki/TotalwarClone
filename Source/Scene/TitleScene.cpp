#include "TitleScene.h"
#include "Actor\Base\GameObject.h"
#include "Component\Graphics\SpriteRenderer.h"
#include "Component\Graphics\D2DTextRenderer.h"
#include "Component\Utility\Action\Actions.h"
#include "Component\Utility\Action\ActionManager.h"
#include "Component\Utility\Action\EasingActions.h"
#include "Device\GameDevice.h"
#include "Utility\ModelGameObjectHelper.h"

#include "Def/Screen.h"

std::string TitleScene::nextScene()
{
	return "SelectScene";
}

bool TitleScene::isEnd()
{
	return m_SceneChangeTimer.isTime();
}

void TitleScene::start()
{
	m_pDefaultCamera->setDrawMode(DrawMode::DrawMode_2D);

	GameObject* pObj = new GameObject(this);
	auto pTextRenderer = pObj->addComponent<D2DTextRenderer>();
	pTextRenderer->setFont(L"Meiryo UI", L"ja-jp", 128.0f);
	pTextRenderer->setFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_MEDIUM);
	pTextRenderer->setFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
	pTextRenderer->setFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
	pTextRenderer->setColor(Color(0.0f, 1.0f, 0.0f, 1.0f));
	pTextRenderer->setTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
	pTextRenderer->setParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	pTextRenderer->setTextRect(0.0f, 0.0f, 1280.0f, 720.0f);
	pTextRenderer->setText(L"タイトル");

	//板の幅
	const float plateWidth = 64.0f;
	//板の高さ
	const float plateHeight = 64.0f;

	//板の数(横)
	const int plateCountX = WindowWidth / plateWidth;
	//板の数(縦)
	const int plateCountY = WindowHeight / plateHeight;

	const float offsetX = -plateWidth * plateCountX / 2 + plateWidth * 0.5f;
	const float offsetY = -plateHeight * plateCountY / 2 + plateHeight * 0.5f;

	//板を並べる
	for (int y = 0; y < plateCountY; y++)
	{
		for (int x = 0; x < plateCountX; x++)
		{
			auto pObj = new GameObject(this);
			pObj->getTransform().setLocalPosition(Vec3(offsetX + x * plateWidth, offsetY + y * plateWidth));
			pObj->getTransform().setLocalScale(Vec3(plateWidth, plateHeight, 1.0f) * 0.9f);

			auto pSpriteRenderer = pObj->addComponent<SpriteRenderer>();
			pSpriteRenderer->setTextureByName("BoxFill");
			pSpriteRenderer->setColor(Color(0.0f, 0.0f, 0.0f, 1.0f));
			m_SpriteRenderers.emplace_back(pSpriteRenderer);

			auto pActionManager = pObj->addComponent<Action::ActionManager>();
			m_ActionManagers.emplace_back(pActionManager);
		}
	}

	m_SceneChangeTimer.setMaxTime(7.5f);
	m_SceneChangeTimer.reset();

	m_IsSceneChangeBegin = false;
}

void TitleScene::update()
{
	if (GameDevice::getInput().isKeyDown(DIK_SPACE) &&
		!m_IsSceneChangeBegin)
	{
		//時間算出用の最大距離
		const float sqrDistanceMax = Vec3(WindowWidth / 2, WindowHeight / 2, 0.0f).sqrLength() * 0.5f;

		//中心から波打たせる
		for (int i = 0; i < (int)m_ActionManagers.size(); i++)
		{
			auto pActionManager = m_ActionManagers.at(i);

			auto pObj = &pActionManager->getUser();
			float sqrDistanceFromCenter = pObj->getTransform().getLocalPosition().sqrLength();
			float time = sqrDistanceFromCenter / sqrDistanceMax * 0.5f;
			const Vec3& localScale = pObj->getTransform().getLocalScale();

			for (int j = 0; j < 3; j++)
			{
				pActionManager->enqueueAction(
					new Action::Spawn(
						{
							new Action::Sequence(
								{
									new Action::WaitForSeconds(time),
									new Action::EaseInBack(new Action::ScaleTo(localScale * 0.5f, 0.5f)),
									new Action::ScaleTo(localScale, 0.25f)
								}
							),
							new Action::Sequence(
								{
									new Action::WaitForSeconds(time),
									new Action::ColorTo(Color(1.0f, 1.0f, 1.0f, 1.0f), m_SpriteRenderers.at(i), 0.5f),	//白くする
									new Action::ColorTo(Color(0.0f, 0.0f, 0.0f, 1.0f), m_SpriteRenderers.at(i), 0.25f)	//黒くする
								}
							),
						}
					)
				);
			}
		}

		m_IsSceneChangeBegin = true;
	}

	if (!m_IsSceneChangeBegin) return;

	m_SceneChangeTimer.update();
}

void TitleScene::shutdown()
{
}
