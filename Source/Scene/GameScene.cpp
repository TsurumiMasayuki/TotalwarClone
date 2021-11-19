#include "GameScene.h"
#include "Component\Graphics\InstancedRenderer.h"
#include "Component\Physics\BoxColliderBt.h"
#include "Device\GameDevice.h"
#include "Utility\ModelGameObjectHelper.h"
#include "Math\MathUtility.h"

#include "AI\ValueMap.h"
#include "AI\ValueMapRenderer.h"

#include "Unit\Unit.h"
#include "Component\Cursor.h"
#include "Component\Box2D\PhysicsManagerB2.h"
#include "Component\Physics\BoxColliderBt.h"

#include "Graphics\DX12\Material\DefaultMaterials.h"
#include "Graphics\Material\ValueMapMaterial.h"
#include "Graphics\Material\BBModelMaterial.h"

#include "Player\Player.h"
#include "Player\AIPlayer\AIPlayer.h"

#include "Unit\UnitRenderHelper.h"
#include "Unit\UnitSelector.h"
#include "Unit\UnitStats.h"

#include "Utility\JsonFileManager.h"

#include "UI\UIUnitList.h"
#include "UI\UIUnitPlacer.h"

#include "Blockbench\BlockbenchModel.h"
#include "Blockbench\BlockbenchLoader.h"

#include "Stage\Stage.h"

#include "GameState.h"

Cursor* g_pCursor;

int g_TeamID1 = 0;
int g_TeamID2 = 1;

//DX12Mesh::MeshVertex vertices[8] =
//{
//	{ DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT2(0.0f, 0.0f) },
//	{ DirectX::XMFLOAT3(1, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT2(0.0f, 0.0f) },
//	{ DirectX::XMFLOAT3(1, 1, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT2(0.0f, 0.0f) },
//	{ DirectX::XMFLOAT3(0, 1, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT2(0.0f, 0.0f) },
//	{ DirectX::XMFLOAT3(0, 0, 1), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT2(0.0f, 0.0f) },
//	{ DirectX::XMFLOAT3(1, 0, 1), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT2(0.0f, 0.0f) },
//	{ DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT2(0.0f, 0.0f) },
//	{ DirectX::XMFLOAT3(0, 1, 1), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT2(0.0f, 0.0f) }
//};

std::string GameScene::nextScene()
{
	return std::string();
}

bool GameScene::isEnd()
{
	return false;
}

void GameScene::start()
{
	Game::g_GameState = Game::GameState::PreparePhase;

	//攻撃ステータスの読み込み
	{
		auto& attackStatsManager = JsonFileManager<AttackStats>::getInstance();
		attackStatsManager.load("TestAttack", "Resources/AttackStats/TestAttack.json");
		attackStatsManager.load("TestAttack_Strong", "Resources/AttackStats/TestAttack_Strong.json");
		attackStatsManager.load("TestAttack_Snipe", "Resources/AttackStats/TestAttack_Snipe.json");
		attackStatsManager.load("CruiserNormalAttack", "Resources/AttackStats/CruiserNormalAttack.json");
	}

	//ユニットステータスの読み込み
	{
		auto& unitStatsManager = JsonFileManager<UnitStats>::getInstance();
		unitStatsManager.load("NormalCorvette", "Resources/UnitStats/NormalCorvette.json");
		unitStatsManager.load("NormalCruiser", "Resources/UnitStats/NormalCruiser.json");
		unitStatsManager.load("NormalBattleship", "Resources/UnitStats/NormalBattleship.json");
		unitStatsManager.load("SniperCruiser", "Resources/UnitStats/SniperCruiser.json");
	}

	//ステージの読み込み
	{
		auto& stageManager = JsonFileManager<Stage>::getInstance();
		stageManager.load("TestStage", "Resources/Stages/TestStage.json");
	}

	//マテリアルの生成
	m_pInstancingMaterial = new InstancingMaterial();
	m_pInstancingMaterial->init(DX12GraphicsCore::g_pDevice.Get());

	//マテリアルの生成
	m_pValueMapMaterial = new ValueMapMaterial();
	m_pValueMapMaterial->init(DX12GraphicsCore::g_pDevice.Get());
	m_pValueMapMaterial->setMainTexture(GameDevice::getTextureManager().getTexture("CircleFill"));

	//マテリアルの生成
	m_pBBModelMaterial = new BBModelMaterial();
	m_pBBModelMaterial->init(DX12GraphicsCore::g_pDevice.Get());

	//物理マネージャーの生成
	auto pPhysicsManagerObj = new GameObject(this);
	pPhysicsManagerObj->addComponent<PhysicsManagerB2>();

	//カーソル用オブジェクト
	auto pCursorObj = ModelGameObjectHelper::instantiateModel<int>(this, GameDevice::getModelManager().getModel("Sphere"));
	pCursorObj->getTransform().setLocalScale(Vec3(1.0f));

	g_pCursor = pCursorObj->addComponent<Cursor>();
	g_pCursor->init(m_pDefaultCamera);

	m_pUnitSelector = pCursorObj->addComponent<UnitSelector>();
	m_pUnitSelector->init(g_pCursor, g_TeamID1, m_pValueMapMaterial);

	//AIプレイヤー1の生成
	auto pPlayer1Obj = new GameObject(this);
	m_pPlayer = pPlayer1Obj->addComponent<Player>();

	//AIプレイヤー2の生成
	auto pPlayer2Obj = new GameObject(this);
	m_pAIPlayer = pPlayer2Obj->addComponent<AIPlayer>();

	m_pPlayer->init(g_TeamID1, m_pAIPlayer, &m_ValueMap2);
	m_pAIPlayer->init(g_TeamID2, m_pPlayer, &m_ValueMap1);

	auto pSphereModel = GameDevice::getModelManager().getModel("Sphere");

	//ユニット描画用オブジェクト生成
	for (const auto& pair : JsonFileManager<UnitStats>::getInstance().getAll())
	{
		auto pRendererObj = ModelGameObjectHelper::instantiateModel<UnitInstanceInfo>(this, pSphereModel, true);
		auto pRenderer = pRendererObj->getChildren().at(0)->getComponent<InstancedRenderer<UnitInstanceInfo>>();
		pRenderer->setMaterial(m_pInstancingMaterial);

		m_UnitRenderHelpers.emplace(pair.first, new UnitRenderHelper(&pair.second, nullptr, pRenderer));
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
		m_pUIUnitPlacer->init(g_pCursor, m_pPlayer, m_pUnitSelector, &m_ValueMap1, &m_UnitRenderHelpers, m_pEffectRenderHelper);

		auto pUIObj2 = new GameObject(this);
		pUIObj2->setParent(&m_pDefaultCamera->getUser());
		m_pUIUnitList = pUIObj2->addComponent<UIUnitList>();
		pUIObj2->setActive(false);
	}

	//ステージ設定読み込み
	const Stage& testStage = JsonFileManager<Stage>::getInstance().get("TestStage");
	m_pPlayer->setEnergy(testStage.m_PlayerEnergy);

	//ユニット生成
	for (const auto& enemy : testStage.getUnitList())
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
	//pValueMapRenderer->init(&m_ValueMap2, m_pValueMapMaterial, Color(DirectX::Colors::LightGreen));

	//レイキャスト判定用平面オブジェクト
	{
		auto pPlaneObj = new GameObject(this);
		pPlaneObj->getTransform().setLocalScale(Vec3(3000.0f, 0.1f, 3000.0f));
		auto pCollider = pPlaneObj->addComponent<BoxColiiderBt>();
		pCollider->setMass(0.0f);
	}

	{
		//std::string filePath = "Resources/Hoge.json";
		//GameDevice::getTextureManager().load("Hoge", L"Resources/Hoge.png");

		//BlockbenchLoader loader;
		//loader.load(filePath, "Hoge", "Hoge");
		//auto& matrices = loader.getModel("Hoge")->getCubeMatrices();

		//std::vector<UnitInstanceInfo> instances;
		//for (auto& matrix : matrices)
		//{
		//	instances.emplace_back();
		//	auto& instance = instances.back();

		//	DirectX::XMStoreFloat4x4(&instance.instanceMat, DirectX::XMMatrixTranspose(matrix));
		//	instance.instanceColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		//}

		//auto& uvOrigins = loader.getModel("Hoge")->getUVOrigins();
		//auto& uvSizes = loader.getModel("Hoge")->getUVSizes();
		//for (int i = 0; i < (int)instances.size(); i++)
		//{
		//	for (int j = 0; j < BlockbenchModel::cubeFaceCount / 2; j++)
		//	{
		//		//原点をセット
		//		instances[i].instanceUVOrigins.m[j][0] = uvOrigins.at(j).x;
		//		instances[i].instanceUVOrigins.m[j][1] = uvOrigins.at(j).y;
		//		instances[i].instanceUVOrigins.m[j][2] = uvOrigins.at(j + 3).x;
		//		instances[i].instanceUVOrigins.m[j][3] = uvOrigins.at(j + 3).y;

		//		//サイズをセット
		//		instances[i].instanceUVSizes.m[j][0] = uvSizes.at(j).x;
		//		instances[i].instanceUVSizes.m[j][1] = uvSizes.at(j).y;
		//		instances[i].instanceUVSizes.m[j][2] = uvSizes.at(j + 3).x;
		//		instances[i].instanceUVSizes.m[j][3] = uvSizes.at(j + 3).y;
		//	}
		//}

		//m_pBBModelMaterial->setMainTexture(GameDevice::getTextureManager().getTexture("BoxFill"));

		//auto pModelObj = new GameObject(this);
		//auto pInstancedRenderer = pModelObj->addComponent<InstancedRenderer<UnitInstanceInfo>>();
		//pInstancedRenderer->setMesh();
		//pInstancedRenderer->setMaterial(m_pBBModelMaterial);
		//pInstancedRenderer->setInstanceInfo(instances);
		//loader.unLoadModels();
	}
}

void GameScene::update()
{
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
}

void GameScene::shutdown()
{
	//マテリアルの削除
	delete m_pInstancingMaterial;
	delete m_pValueMapMaterial;
	delete m_pBBModelMaterial;

	for (auto& pair : m_UnitRenderHelpers)
	{
		delete pair.second;
	}

	delete m_pEffectRenderHelper;
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
