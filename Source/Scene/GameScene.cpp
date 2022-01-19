#include "GameScene.h"
#include "Component\Graphics\InstancedRenderer.h"
#include "Component\Physics\BoxColliderBt.h"
#include "Def\Screen.h"
#include "Device\GameDevice.h"
#include "Utility\ModelGameObjectHelper.h"
#include "Math\MathUtility.h"

#include "AI\ValueMap.h"
#include "AI\ValueMapRenderer.h"

#include "Blockbench\BlockbenchModel.h"
#include "Blockbench\BlockbenchLoader.h"

#include "Component\Cursor.h"
#include "Component\Box2D\PhysicsManagerB2.h"
#include "Component\Graphics\D2DTextRenderer.h"
#include "Component\Graphics\GUI\GUISpriteRenderer.h"
#include "Component\Physics\BoxColliderBt.h"
#include "Component\Utility\Action\ActionManager.h"
#include "Component\Utility\Action\Actions.h"

#include "Component\Skybox.h"

#include "Graphics\DX12\Material\DefaultMaterials.h"
#include "Graphics\Material\ValueMapMaterial.h"
#include "Graphics\Material\BBModelMaterial.h"

#include "Player\Player.h"
#include "Player\AIPlayer\AIPlayer.h"

#include "Unit\Unit.h"
#include "Unit\UnitRenderHelper.h"
#include "Unit\UnitSelector.h"
#include "Unit\UnitStats.h"

#include "Utility\JsonFileManager.h"

#include "UI\UIBattleSlider.h"
#include "UI\UIUnitList.h"
#include "UI\UIUnitPlacer.h"
#include "UI\UIUnitInfo.h"

#include "Sound\SEManager.h"
#include "Stage\Stage.h"

#include "GameState.h"

int g_TeamID1 = 0;
int g_TeamID2 = 1;

std::string GameScene::nextScene()
{
	return "TitleScene";
}

bool GameScene::isEnd()
{
	//どちらかが敗北したら終了
	return m_SceneChangeTimer.isTime() && Game::g_GameState == Game::GameState::Result &&
		(m_pPlayer->isDefeat() || m_pAIPlayer->isDefeat());
}

void GameScene::start()
{
	//スカイボックス
	auto pBGObject = new GameObject(this);
	auto pBGSprite = pBGObject->addComponent<Skybox>();

	//ステージ設定読み込み
	const Stage& stage = JsonFileManager<Stage>::getInstance().get(m_StageName);

	//板の幅
	const float plateWidth = 64.0f;
	//板の高さ
	const float plateHeight = 64.0f;

	//板の数(横)
	const int plateCountX = (int)(WindowWidth / plateWidth);
	//板の数(縦)
	const int plateCountY = (int)(WindowHeight / plateHeight) + 1;

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
			pObj->getTransform().setLocalPosition(Vec3(offsetX + plateWidth * x, offsetY + plateHeight * y, 0.3f));
			pObj->getTransform().setLocalScale(Vec3(plateWidth, plateHeight, 1.0f));

			//黒い画像を設定
			auto pRenderer = pObj->addComponent<GUISpriteRenderer>();
			pRenderer->setTextureByName("BoxFill");
			pRenderer->setColor(Color(0.0f, 0.0f, 0.0f, 1.0f));

			auto pActionManager = pObj->addComponent<Action::ActionManager>();

			//イージング
			float ease = 1.0f - Easing::easeInQuad((float)x / (float)plateCountX);
			pActionManager->enqueueAction(
				new Action::WaitForSeconds(3.0f * ease)
			);

			//黒い画像を白くしながら消す
			pActionManager->enqueueAction(
				new Action::Spawn(
					{
						new Action::EaseInExpo(
							new Action::ScaleTo(
								Vec3(0.0f, 0.0f, 1.0f), 2.0f
							)
						),
						new Action::ColorTo(
							Color(1.0f, 1.0f, 1.0f, 1.0f),
							pRenderer,
							1.8f
						),
						new Action::EaseInExpo(
							new Action::MoveTo(
								Vec3(offsetX + plateWidth * x * ratioX, offsetY + plateHeight * y * ratioY, 1.0f),
								2.0f
							)
						)
					}
				)
			);

			pActionManager->enqueueAction(new Action::Destroy(0.0f));
		}
	}

	m_SpriteRenderers.clear();
	m_ActionManagers.clear();

	//板を並べる
	for (int y = 0; y < plateCountY; y++)
	{
		for (int x = 0; x < plateCountX; x++)
		{
			auto pObj = new GameObject(this);
			pObj->setParent(&m_pDefaultCamera->getUser());
			pObj->getTransform().setLocalPosition(Vec3(offsetX + x * plateWidth, offsetY + y * plateHeight, 0.5f));
			pObj->getTransform().setLocalScale(Vec3(plateWidth, plateHeight, 1.0f));

			auto pSpriteRenderer = pObj->addComponent<GUISpriteRenderer>();
			pSpriteRenderer->setTextureByName("BoxFill");
			pSpriteRenderer->setColor(Color(1.0f, 1.0f, 1.0f, 0.0f));
			m_SpriteRenderers.emplace_back(pSpriteRenderer);

			auto pActionManager = pObj->addComponent<Action::ActionManager>();
			m_ActionManagers.emplace_back(pActionManager);
		}
	}

	//SEManagerの設定
	SEManager::getInstance().setListner(&m_pDefaultCamera->getTransform());

	Game::g_GameState = Game::GameState::PreparePhase;

	//マテリアルの生成
	m_pInstancingMaterial = new InstancingMaterial();
	m_pInstancingMaterial->init(DX12GraphicsCore::g_pDevice.Get());

	//マテリアルの生成
	m_pValueMapMaterial = new ValueMapMaterial();
	m_pValueMapMaterial->init(DX12GraphicsCore::g_pDevice.Get());
	m_pValueMapMaterial->setMainTexture(GameDevice::getTextureManager().getTexture("CircleFill"));

	//物理マネージャーの生成
	auto pPhysicsManagerObj = new GameObject(this);
	pPhysicsManagerObj->addComponent<PhysicsManagerB2>();

	//カーソル用オブジェクト
	auto pCursorObj = ModelGameObjectHelper::instantiateModel<int>(this, GameDevice::getModelManager().getModel("Sphere"));
	pCursorObj->getTransform().setLocalScale(Vec3(1.0f));

	auto pCursor = pCursorObj->addComponent<Cursor>();
	pCursor->init(m_pDefaultCamera);

	//ユニット情報表示UI
	auto pUnitInfoObj = new GameObject(this);
	pUnitInfoObj->setParent(&m_pDefaultCamera->getUser());
	auto pUIUnitInfo = pUnitInfoObj->addComponent<UIUnitInfo>();

	//ユニット選択補助
	m_pUnitSelector = pCursorObj->addComponent<UnitSelector>();
	m_pUnitSelector->init(pCursor, g_TeamID1, m_pValueMapMaterial, pUIUnitInfo);

	//AIプレイヤー1の生成
	auto pPlayer1Obj = new GameObject(this);
	m_pPlayer = pPlayer1Obj->addComponent<Player>();

	//AIプレイヤー2の生成
	auto pPlayer2Obj = new GameObject(this);
	m_pAIPlayer = pPlayer2Obj->addComponent<AIPlayer>();

	m_pPlayer->init(g_TeamID1, m_pAIPlayer, &m_ValueMap2);
	m_pAIPlayer->init(g_TeamID2, m_pPlayer, &m_ValueMap1);

	auto pSphereModel = GameDevice::getModelManager().getModel("Sphere");

	{
		DX12Mesh::MeshVertex baseVertices[8] =
		{
			{ { 0.0f, 1.0f, 1.0f }, {  0.0f,  0.0f, 1.0f }, { 0.625f, 0.5f } },
			{ { 0.0f, 0.0f, 1.0f }, {  0.0f,  0.0f, 1.0f }, { 0.375f, 0.5f } },
			{ { 0.0f, 1.0f, 0.0f }, { -1.0f,  0.0f, 0.0f }, { 0.625f, 0.75f } },
			{ { 0.0f, 0.0f, 0.0f }, { -1.0f,  0.0f, 0.0f }, { 0.375f, 0.75f } },
			{ { 1.0f, 1.0f, 1.0f }, {  0.0f,  0.0f, 1.0f }, { 0.625f, 0.25f } },
			{ { 1.0f, 0.0f, 1.0f }, {  0.0f,  0.0f, 1.0f }, { 0.375f, 0.25f } },
			{ { 1.0f, 1.0f, 0.0f }, {  1.0f,  0.0f, 0.0f }, { 0.625f, 0.0f } },
			{ { 1.0f, 0.0f, 0.0f }, {  0.0f, -1.0f, 0.0f }, { 0.125f, 0.75f } }
		};

		int baseIndices[36] =
		{
			0, 4, 6, 6, 2, 0,
			3, 2, 6, 6, 7, 3,
			7, 6, 4, 4, 5, 7,
			5, 1, 3, 3, 7, 5,
			1, 0, 2, 2, 3, 1,
			5, 4, 0, 0, 1, 5
		};

		std::vector<DX12Mesh::MeshVertex> vertices;
		std::vector<USHORT> indices;
		vertices.resize(36);
		indices.resize(36);
		for (int i = 0; i < 36; i++)
		{
			vertices[i] = baseVertices[baseIndices[i]];
			indices[i] = i;
		}

		m_pBBCube = new DX12Mesh();
		m_pBBCube->init(DX12GraphicsCore::g_pDevice.Get(), vertices, indices, "BoxFill");
	}

	GameDevice::getTextureManager().load("Hoge", L"Resources/Hoge.png");
	m_BBModelLoader.load("Resources/Hoge.json", "Hoge", "Hoge");

	GameDevice::getTextureManager().load("HogeHoge", L"Resources/HogeHoge.png");
	m_BBModelLoader.load("Resources/HogeHoge.json", "HogeHoge", "HogeHoge");

	//ユニット描画用オブジェクト生成
	for (const auto& pair : JsonFileManager<UnitStats>::getInstance().getAll())
	{
		const auto bbModel = m_BBModelLoader.getModel(pair.second.m_ModelName);

		auto pRendererObj = new GameObject(this);
		auto pRenderer = pRendererObj->addComponent<InstancedRenderer<UnitInstanceInfo>>();
		pRenderer->setMesh(m_pBBCube);

		m_UnitRenderHelpers.emplace(pair.first, new UnitRenderHelper(&pair.second, bbModel, pRenderer));
	}
	
	auto pCubeModel = GameDevice::getModelManager().getModel("Cube");

	//エフェクト描画用オブジェクト生成
	{
		auto pRendererObj = ModelGameObjectHelper::instantiateModel<EffectInstanceInfo>(this, pCubeModel, true);
		auto pRenderer = pRendererObj->getChildren().at(0)->getComponent<InstancedRenderer<EffectInstanceInfo>>();
		pRenderer->setMaterial(m_pInstancingMaterial);

		m_pEffectRenderHelper = new EffectRenderHelper(pRenderer);
	}

	//UI生成
	{
		auto pUIObj1 = new GameObject(this);
		pUIObj1->setParent(&m_pDefaultCamera->getUser());
		m_pUIUnitPlacer = pUIObj1->addComponent<UIUnitPlacer>();
		m_pUIUnitPlacer->init(pCursor, m_pPlayer, m_pUnitSelector, stage, pUIUnitInfo, &m_ValueMap1, &m_UnitRenderHelpers, m_pEffectRenderHelper);

		auto pUIObj2 = new GameObject(this);
		pUIObj2->setParent(&m_pDefaultCamera->getUser());
		m_pUIUnitList = pUIObj2->addComponent<UIUnitList>();
		pUIObj2->setActive(false);
	}

	//プレイヤーの使用エネルギー設定
	m_pPlayer->setEnergy(stage.m_PlayerEnergy);

	//ユニット生成
	for (const auto& enemy : stage.getUnitList())
	{
		const UnitStats& unitStats = JsonFileManager<UnitStats>::getInstance().get(enemy.m_Name);

		GameObject* pUnitObj = new GameObject(this);
		auto pUnit = pUnitObj->addComponent<Unit>();
		pUnit->init(m_pAIPlayer, &unitStats, &m_ValueMap2, m_UnitRenderHelpers.at(unitStats.m_Name), m_pEffectRenderHelper, false);
		pUnit->setPosition(enemy.m_Position, enemy.m_Angle, enemy.m_Width);

		m_pAIPlayer->addUnit(pUnit);
	}

	//情報マップ描画の生成
	//auto pValueMapRendererObj = new GameObject(this);
	//auto pValueMapRenderer = pValueMapRendererObj->addComponent<ValueMapRenderer<UnitStatsValues::Health>>();
	//pValueMapRenderer->setUnit(&m_ValueMap2, m_pValueMapMaterial, Color(DirectX::Colors::LightGreen));

	//レイキャスト判定用平面オブジェクト
	{
		auto pPlaneObj = new GameObject(this);
		pPlaneObj->getTransform().setLocalScale(Vec3(3000.0f, 0.1f, 3000.0f));
		auto pCollider = pPlaneObj->addComponent<BoxColiiderBt>();
		pCollider->setMass(0.0f);
	}

	//勝敗テキスト
	{
		auto pTextObj = new GameObject(this);

		//TextRendererの設定
		m_pWinLoseText = pTextObj->addComponent<D2DTextRenderer>();
		m_pWinLoseText->setFont(L"Meiryo UI", L"ja-jp", 48.0f);
		m_pWinLoseText->setFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_MEDIUM);
		m_pWinLoseText->setFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
		m_pWinLoseText->setFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
		m_pWinLoseText->setColor(Color(0.0f, 0.0f, 0.0f, 1.0f));
		m_pWinLoseText->setTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
		m_pWinLoseText->setParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		m_pWinLoseText->setTextRect(0.0f, 0.0f, 1280.0f, 720.0f);
		m_pWinLoseText->setText(L"");
	}

	//戦力値表示
	{
		auto pUIObj = new GameObject(this);
		pUIObj->setParent(&m_pDefaultCamera->getUser());

		auto pUIBattleSlider = pUIObj->addComponent<UIBattleSlider>();
		pUIBattleSlider->init(m_pPlayer, m_pAIPlayer);
	}
	
	m_IsSceneChangeBegin = false;
	m_SceneChangeTimer.setMaxTime(5.5f);
	m_SceneChangeTimer.reset();
}

void GameScene::update()
{
	//SEManagerの更新
	SEManager::getInstance().update();

	//CombatPhaseBeginを1フレームで終わらせる
	if (Game::g_GameState == Game::GameState::CombatPhaseBegin)
	{
		Game::g_GameState = Game::GameState::CombatPhase;

		m_pUIUnitPlacer->getUser().setActive(false);
		m_pUIUnitList->getUser().setActive(true);
		m_pUIUnitList->init(m_pPlayer, m_pUnitSelector);
	}

	if (GameDevice::getInput().isKeyDown(DIK_1))
	{
		Game::g_GameState = Game::GameState::CombatPhaseBegin;
	}

	//戦闘フェーズ内でどちらかが負けたらステート変更
	if (Game::g_GameState == Game::GameState::CombatPhase &&
		(m_pPlayer->isDefeat() || m_pAIPlayer->isDefeat()))
	{
		Game::g_GameState = Game::GameState::Result;
	}

	if (Game::g_GameState == Game::GameState::Result)
	{
		//プレイヤーの勝敗によってテキスト表示
		m_pWinLoseText->setText(!m_pPlayer->isDefeat() ? L"Win!" : L"Lose...");
	}

	//スペースキーが押されたらカメラ移動
	if (GameDevice::getInput().isKey(DIK_SPACE))
	{
		auto& cameraTr = m_pDefaultCamera->getTransform();

		auto mousePos = GameDevice::getInput().getMouseMove();
		cameraTr.setLocalAngles(cameraTr.getLocalAngles() + Vec3(mousePos.y, mousePos.x, 0.0f).normalized());

		const auto& input = GameDevice::getInput();
		Vec3 move;
		if (input.isKey(DIK_W)) move.z += 1.0f;
		if (input.isKey(DIK_S)) move.z -= 1.0f;
		if (input.isKey(DIK_E)) move.y += 1.0f;
		if (input.isKey(DIK_Q)) move.y -= 1.0f;
		if (input.isKey(DIK_A)) move.x -= 1.0f;
		if (input.isKey(DIK_D)) move.x += 1.0f;

		move = move.multMatrix(cameraTr.getRotationMatrix());
		cameraTr.setLocalPosition(cameraTr.getLocalPosition() + move);
	}

	//シーン終了演出
	if (GameDevice::getInput().isKeyDown(DIK_2) &&
		Game::g_GameState == Game::GameState::Result &&
		!m_IsSceneChangeBegin)
	{
		m_IsSceneChangeBegin = true;

		//時間算出用の最大距離
		const float sqrDistanceMax = Vec3(WindowWidth / 2, WindowHeight / 2, 0.0f).sqrLength() * 0.5f;

		for (int i = 0; i < (int)m_ActionManagers.size(); i++)
		{
			auto pActionManager = m_ActionManagers.at(i);

			auto pObj = &pActionManager->getUser();
			float sqrDistanceFromCenter = pObj->getTransform().getLocalPosition().sqrLength();
			float time = (1.0f - sqrDistanceFromCenter / sqrDistanceMax) + 1.0f;
			const Vec3& localScale = pObj->getTransform().getLocalScale();

			pActionManager->enqueueAction(
				new Action::Spawn(
					{
						new Action::Sequence(
							{
								new Action::WaitForSeconds(time),
								new Action::ColorTo(Color(0.0f, 0.0f, 0.0f, 1.0f), m_SpriteRenderers.at(i), 0.5f),	//黒くする
							}
						),
					}
				)
			);
		}
	}

	if (!m_IsSceneChangeBegin) return;

	m_SceneChangeTimer.update();
}

void GameScene::shutdown()
{
	//マテリアルの削除
	delete m_pInstancingMaterial;
	delete m_pValueMapMaterial;

	delete m_pBBCube;

	for (auto& pair : m_UnitRenderHelpers)
	{
		delete pair.second;
	}

	m_UnitRenderHelpers.clear();

	delete m_pEffectRenderHelper;

	SEManager::getInstance().clear();

	Game::g_GameState = Game::GameState::PreparePhase;

	m_BBModelLoader.unLoadModels();
}

void GameScene::lateUpdate()
{
	//情報マップをクリア
	m_ValueMap1.clearAll();
	m_ValueMap2.clearAll();

	//InstancedRendererに一斉転送
	for (auto& pair : m_UnitRenderHelpers)
	{
		pair.second->sendInstanceInfo();
	}

	m_pEffectRenderHelper->sendInstanceInfo();
}

void GameScene::setStage(const std::string& stageName)
{
	m_StageName = stageName;
}
