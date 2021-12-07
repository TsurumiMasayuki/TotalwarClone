#include "SelectScene.h"
#include "Component\Utility\Action\Actions.h"
#include "Component\Utility\Action\ActionManager.h"
#include "Component\Utility\Action\EasingActions.h"
#include "Component\Graphics\SpriteRenderer.h"
#include "Device\GameDevice.h"
#include "Scene\GameScene.h"
#include "UI\UIStageList.h"
#include "Def\Screen.h"

SelectScene::SelectScene(GameScene* pGameScene)
	: m_pGameScene(pGameScene)
{
}

std::string SelectScene::nextScene()
{
	return "GameScene";
}

bool SelectScene::isEnd()
{
	if (m_pActionManager == nullptr)
		return false;

	return m_IsChangeScene &&
		m_pActionManager->actionCount() == 0;
}

void SelectScene::start()
{
	auto pObj = new GameObject(this);
	m_pUIStageList = pObj->addComponent<UIStageList>();

	m_pDefaultCamera->setDrawMode(DrawMode::DrawMode_2D);

	m_IsChangeScene = false;
	m_pActionManager = nullptr;

	//板の幅
	const float plateWidth = 64.0f;
	//板の高さ
	const float plateHeight = 64.0f;

	//板の数(横)
	const int plateCountX = WindowWidth / plateWidth;
	//板の数(縦)
	const int plateCountY = WindowHeight / plateHeight + 1;

	const float offsetX = -plateWidth * plateCountX / 2 + plateWidth * 0.5f;
	const float offsetY = -plateHeight * plateCountY / 2 + plateHeight * 0.5f;

	m_SpriteRenderers.clear();
	m_ActionManagers.clear();

	//板を並べる
	for (int y = 0; y < plateCountY; y++)
	{
		for (int x = 0; x < plateCountX; x++)
		{
			auto pObj = new GameObject(this);
			pObj->getTransform().setLocalPosition(Vec3(offsetX + x * plateWidth, offsetY + y * plateWidth));
			pObj->getTransform().setLocalScale(Vec3(plateWidth, plateHeight, 1.0f));

			auto pSpriteRenderer = pObj->addComponent<SpriteRenderer>();
			pSpriteRenderer->setTextureByName("BoxFill");
			pSpriteRenderer->setColor(Color(0.0f, 0.0f, 0.0f, 1.0f));
			m_SpriteRenderers.emplace_back(pSpriteRenderer);

			auto pActionManager = pObj->addComponent<Action::ActionManager>();
			m_ActionManagers.emplace_back(pActionManager);
		}
	}

	//時間算出用の最大距離
	const float sqrDistanceMax = Vec3(WindowWidth / 2, WindowHeight / 2, 0.0f).sqrLength() * 0.5f;

	for (int i = 0; i < (int)m_ActionManagers.size(); i++)
	{
		auto pActionManager = m_ActionManagers.at(i);

		auto pObj = &pActionManager->getUser();
		float sqrDistanceFromCenter = pObj->getTransform().getLocalPosition().sqrLength();
		float time = sqrDistanceFromCenter / sqrDistanceMax + 1.0f;
		const Vec3& localScale = pObj->getTransform().getLocalScale();

		pActionManager->enqueueAction(
			new Action::Spawn(
				{
					new Action::Sequence(
						{
							new Action::WaitForSeconds(time),
							new Action::ScaleTo(Vec3(0.0f, 0.0f, 1.0f), 0.5f)
						}
					),
					new Action::Sequence(
						{
							new Action::WaitForSeconds(time),
							new Action::ColorTo(Color(1.0f, 1.0f, 1.0f, 1.0f), m_SpriteRenderers.at(i), 0.5f),	//白くする
						}
					),
				}
			)
		);

		pActionManager->enqueueAction(
			new Action::Destroy(0.0f)
		);
	}
}

void SelectScene::update()
{
	if (GameDevice::getInput().isKeyDown(DIK_SPACE) &&
		!m_IsChangeScene)
	{
		m_IsChangeScene = true;

		//板の幅
		const float plateWidth = 64.0f;
		//板の高さ
		const float plateHeight = 64.0f;

		//板の数(横)
		const int plateCountX = WindowWidth / plateWidth;
		//板の数(縦)
		const int plateCountY = WindowHeight / plateHeight + 1;

		const float offsetX = -plateWidth * plateCountX / 2 + plateWidth * 0.5f;
		const float offsetY = -plateHeight * plateCountY / 2 + plateHeight * 0.5f;

		//ゲーム開始時演出
		for (int y = 0; y < plateCountY; y++)
		{
			for (int x = 0; x < plateCountX; x++)
			{
				float ratioX = 1.0f + ((float)x - (float)plateCountX * 0.5f) / plateCountX * 0.5f;
				float ratioY = 1.0f + ((float)y - (float)plateCountY * 0.5f) / plateCountY * 0.5f;

				auto pObj = new GameObject(this);
				pObj->setParent(&m_pDefaultCamera->getUser());
				pObj->getTransform().setLocalPosition(Vec3(offsetX + plateWidth * x * ratioX, offsetY + plateHeight * y * ratioY, 1.0f));
				pObj->getTransform().setLocalScale(Vec3(0.0f, 0.0f, 1.0f));

				//白い画像を設定
				auto pRenderer = pObj->addComponent<SpriteRenderer>();
				pRenderer->setTextureByName("BoxFill");
				pRenderer->setColor(Color(1.0f, 1.0f, 1.0f, 1.0f));

				auto pActionManager = pObj->addComponent<Action::ActionManager>();

				//イージング
				float ease = 1.0f - Easing::easeInQuad((float)x / (float)plateCountX);
				pActionManager->enqueueAction(
					new Action::WaitForSeconds(3.0f * ease)
				);

				//白い画像を黒くしながら出す
				pActionManager->enqueueAction(
					new Action::Spawn(
						{
							new Action::EaseOutExpo(
								new Action::ScaleTo(
									Vec3(plateWidth * 1.1f, plateHeight * 1.1f, 1.0f), 2.0f
								)
							),
							new Action::ColorTo(
								Color(0.0f, 0.0f, 0.0f, 1.0f),
								pRenderer,
								1.0f
							),
							new Action::EaseOutExpo(
								new Action::MoveTo(
									Vec3(offsetX + plateWidth * x, offsetY + plateHeight * y, 1.0f),
									2.0f
								)
							)
						}
					)
				);

				pActionManager->enqueueAction(
					new Action::WaitForSeconds(1.5f)
				);

				m_pActionManager = pActionManager;
			}
		}
	}
}

void SelectScene::shutdown()
{
	//セレクトされているステージを渡す
	m_pGameScene->setStage(m_pUIStageList->getSelectedStageName());
}
