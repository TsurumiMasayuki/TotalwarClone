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

#include "GameState.h"

Cursor* g_pCursor;

int g_TeamID1 = 0;
int g_TeamID2 = 1;

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
	}

	//ユニットステータスの読み込み
	{
		auto& unitStatsManager = JsonFileManager<UnitStats>::getInstance();
		unitStatsManager.load("NormalCorvette", "Resources/UnitStats/NormalCorvette.json");
		unitStatsManager.load("NormalBattleship", "Resources/UnitStats/NormalBattleship.json");
		unitStatsManager.load("SniperCruiser", "Resources/UnitStats/SniperCruiser.json");
	}

	//マテリアルの生成
	m_pInstancingMaterial = new InstancingMaterial();
	m_pInstancingMaterial->init(DX12GraphicsCore::g_pDevice.Get());

	//マテリアルの生成
	m_pValueMapMaterial = new ValueMapMaterial();
	m_pValueMapMaterial->init(DX12GraphicsCore::g_pDevice.Get());

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

	UnitSelector* pSelector = pCursorObj->addComponent<UnitSelector>();
	pSelector->init(g_pCursor, g_TeamID1, m_pValueMapMaterial);

	auto pModel = GameDevice::getModelManager().getModel("Sphere");

	const auto pUnitStats1 = &JsonFileManager<UnitStats>::getInstance().get("NormalCorvette");
	const auto pUnitStats2 = &JsonFileManager<UnitStats>::getInstance().get("NormalBattleship");
	const auto pUnitStats3 = &JsonFileManager<UnitStats>::getInstance().get("SniperCruiser");

	//AIプレイヤー1の生成
	auto pPlayer1Obj = new GameObject(this);
	pPlayer = pPlayer1Obj->addComponent<Player>();

	//AIプレイヤー2の生成
	auto pPlayer2Obj = new GameObject(this);
	auto pAIPlayer = pPlayer2Obj->addComponent<AIPlayer>();

	pPlayer->init(g_TeamID1, pAIPlayer, &m_ValueMap2);
	pAIPlayer->init(g_TeamID2, pPlayer, &m_ValueMap1);

	//ユニット描画用オブジェクト生成
	for (const auto& pair : JsonFileManager<UnitStats>::getInstance().getAll())
	{
		auto pRendererObj = ModelGameObjectHelper::instantiateModel<UnitInstanceInfo>(this, pModel, true);
		auto pRenderer = pRendererObj->getChildren().at(0)->getComponent<InstancedRenderer<UnitInstanceInfo>>();
		pRenderer->setMaterial(m_pInstancingMaterial);

		m_UnitRenderHelpers.emplace(pair.first, new UnitRenderHelper(&pair.second, nullptr, pRenderer));
	}

	//UI生成
	{
		auto pUIObj = new GameObject(this);
		pUIObj->setParent(&m_pDefaultCamera->getUser());
		UIUnitList* pUnitList = pUIObj->addComponent<UIUnitList>();
		pUnitList->init(pPlayer, pSelector);

		UIUnitPlacer* pUnitPlacer = pUIObj->addComponent<UIUnitPlacer>();
		pUnitPlacer->init(g_pCursor, pPlayer, &m_ValueMap1, &m_UnitRenderHelpers);
	}

	//ユニット生成
	for (int i = -2; i < 3; i++)
	{
		GameObject* pUnitObj = new GameObject(this);
		auto pUnit = pUnitObj->addComponent<Unit>();
		pUnit->init(pAIPlayer, pUnitStats1, &m_ValueMap2, m_UnitRenderHelpers.at(pUnitStats1->m_Name));
		pUnit->setPosition(Vec3(200.0f * i, 0.0f, 200.0f), 180.0f, 10);

		pAIPlayer->addUnit(pUnit);
	}

	//情報マップ描画の生成
	auto pValueMapRendererObj = new GameObject(this);
	auto pValueMapRenderer = pValueMapRendererObj->addComponent<ValueMapRenderer<UnitStatsValues::Health>>();
	pValueMapRenderer->init(&m_ValueMap2, m_pValueMapMaterial, Color(DirectX::Colors::LightGreen));

	//レイキャスト判定用平面オブジェクト
	{
		auto pPlaneObj = new GameObject(this);
		pPlaneObj->getTransform().setLocalScale(Vec3(3000.0f, 0.1f, 3000.0f));
		auto pCollider = pPlaneObj->addComponent<BoxColiiderBt>();
		pCollider->setMass(0.0f);
	}

	{
		//std::string filePath = "Resources/TextureTest.json";
		//GameDevice::getTextureManager().load("TextureTest", L"Resources/TextureTest.png");

		//BlockbenchLoader loader;
		//loader.load(filePath, "TextureTest", "TextureTest");
		//auto& matrices = loader.getModel("TextureTest")->getCubeMatrices();

		//std::vector<UnitInstanceInfo> instances;
		//for (auto& matrix : matrices)
		//{
		//	instances.emplace_back();
		//	auto& instance = instances.back();

		//	DirectX::XMStoreFloat4x4(&instance.instanceMat, DirectX::XMMatrixTranspose(matrix));
		//	instance.instanceColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		//}

		//auto& uvOrigins = loader.getModel("TextureTest")->getUVOrigins();
		//auto& uvSizes = loader.getModel("TextureTest")->getUVSizes();
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

		//m_pBBModelMaterial->setMainTexture(GameDevice::getTextureManager().getTexture("TextureTest"));

		//auto pModelObj = ModelGameObjectHelper::instantiateModel<UnitInstanceInfo>(this, GameDevice::getModelManager().getModel("Cube"), true);
		//auto pInstancedRenderer = pModelObj->getChildren().at(0)->getComponent<InstancedRenderer<UnitInstanceInfo>>();
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
}
