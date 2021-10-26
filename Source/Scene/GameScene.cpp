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
#include "Component\UnitSelector.h"
#include "Component\Box2D\PhysicsManagerB2.h"
#include "Component\Physics\BoxColliderBt.h"

#include "Graphics\DX12\Material\DefaultMaterials.h"
#include "Graphics\Material\ValueMapMaterial.h"
#include "Graphics\Material\BBModelMaterial.h"

#include "Player\AIPlayer\AIPlayer.h"

#include "Unit\UnitStats.h"
#include "Utility\JsonFileManager.h"

#include "UI\UISlider.h"
#include "UI\UIUnitCard.h"

#include "Blockbench\BlockbenchModel.h"
#include "Blockbench\BlockbenchLoader.h"

AIPlayer* pPlayer1;

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

	auto pModel = GameDevice::getModelManager().getModel("Sphere");

	const auto pUnitStats1 = &JsonFileManager<UnitStats>::getInstance().get("NormalCorvette");
	const auto pUnitStats2 = &JsonFileManager<UnitStats>::getInstance().get("NormalBattleship");
	const auto pUnitStats3 = &JsonFileManager<UnitStats>::getInstance().get("SniperCruiser");

	//AIプレイヤー1の生成
	auto pPlayer1Obj = new GameObject(this);
	pPlayer1 = pPlayer1Obj->addComponent<AIPlayer>();
	pPlayer1->setActive(false);

	//AIプレイヤー2の生成
	auto pPlayer2Obj = new GameObject(this);
	auto pPlayer2 = pPlayer2Obj->addComponent<AIPlayer>();
	pPlayer2->setActive(false);

	{
		auto pObj1 = ModelGameObjectHelper::instantiateModel<UnitInstanceInfo>(this, pModel, true);
		auto pObj2 = ModelGameObjectHelper::instantiateModel<UnitInstanceInfo>(this, pModel, true);

		//でかいやつ
		pObj1->getChildren().at(0)->getComponent<InstancedRenderer<UnitInstanceInfo>>()->setMaterial(m_pInstancingMaterial);
		auto pUnit1 = pObj1->getChildren().at(0)->addComponent<Unit>();
		pUnit1->init(g_TeamID1, pUnitStats2, &m_ValueMap1);
		pUnit1->setPosition(Vec3(0.0f, 0.0f, 0.0f), 0.0f, 1);

		//中ぐらいのやつ
		pObj2->getChildren().at(0)->getComponent<InstancedRenderer<UnitInstanceInfo>>()->setMaterial(m_pInstancingMaterial);
		auto pUnit2 = pObj2->getChildren().at(0)->addComponent<Unit>();
		pUnit2->init(g_TeamID1, pUnitStats3, &m_ValueMap1);
		pUnit2->setPosition(Vec3(100.0f, 0.0f, 0.0f), 0.0f, 10);

		//pPlayer1->init(g_TeamID1, pPlayer2, &m_ValueMap2);
		//pPlayer1->addUnit(pUnit1);
	}

	{
		auto pObj1 = ModelGameObjectHelper::instantiateModel<UnitInstanceInfo>(this, pModel, true);

		//小さいやつ
		pObj1->getChildren().at(0)->getComponent<InstancedRenderer<UnitInstanceInfo>>()->setMaterial(m_pInstancingMaterial);
		auto pUnit1 = pObj1->getChildren().at(0)->addComponent<Unit>();
		pUnit1->init(g_TeamID2, pUnitStats1, &m_ValueMap2);
		pUnit1->setPosition(Vec3(0.0f, 0.0f, 200.0f), 180.0f, 10);

		//pPlayer2->init(1, pPlayer1, &m_ValueMap1);
		//pPlayer2->addUnit(pUnit1);

		//ユニットのステータス表示
		{
			m_pUnit1 = pUnit1;

			//ステータス表示用親オブジェクト
			auto pStatsDisplayObj = new GameObject(this);
			pStatsDisplayObj->setParent(&m_pDefaultCamera->getUser());
			pStatsDisplayObj->getTransform().setLocalPosition(Vec3(0.0f, -300.0f, 1.0f));
			pStatsDisplayObj->getTransform().setLocalScale(Vec3(1.0f));

			//ユニットの画像(今は色だけ)
			auto pUnitCardObj = new GameObject(this);
			pUnitCardObj->setParent(pStatsDisplayObj);
			pUnitCardObj->getTransform().setLocalPosition(Vec3(0.0f, 0.0f));
			pUnitCardObj->getTransform().setLocalScale(Vec3(96.0f, 128.0f, 1.0f));
			auto pUnitCard = pUnitCardObj->addComponent<UIUnitCard>();
			pUnitCard->init(pUnit1);

			//ユニットの体力表示
			auto pSliderObj = new GameObject(this);
			pSliderObj->setParent(pStatsDisplayObj);
			pSliderObj->getTransform().setLocalPosition(Vec3(0.0f, 64.0f + 16.0f));
			pSliderObj->getTransform().setLocalScale(Vec3(96.0f, 32.0f, 1.0f));
			m_pHealthSlider = pSliderObj->addComponent<UISlider>();
			m_pHealthSlider->setWidth(1.0f);
			m_pHealthSlider->setDirection(UISlider::Direction::RIGHT);
			m_pHealthSlider->setCurrentValue(pUnit1->getHealth());
			m_pHealthSlider->setMaxValue(pUnit1->getHealth());
			m_pHealthSlider->setTextureByName("BoxFill");
		}
	}

	//情報マップ描画の生成
	auto pValueMapRendererObj = new GameObject(this);
	auto pValueMapRenderer = pValueMapRendererObj->addComponent<ValueMapRenderer<UnitStatsValues::Health>>();
	pValueMapRenderer->init(&m_ValueMap2, m_pValueMapMaterial, Color(DirectX::Colors::LightGreen));

	//レイキャスト判定用平面オブジェクト
	{
		auto pPlaneObj = new GameObject(this);
		pPlaneObj->getTransform().setLocalScale(Vec3(500.0f, 0.1f, 500.0f));
		auto pCollider = pPlaneObj->addComponent<BoxColiiderBt>();
		pCollider->setMass(0.0f);
	}

	//カーソル用オブジェクト
	{
		auto pCursorObj = ModelGameObjectHelper::instantiateModel<int>(this, GameDevice::getModelManager().getModel("Sphere"));
		pCursorObj->getTransform().setLocalScale(Vec3(0.1f));

		g_pCursor = pCursorObj->addComponent<Cursor>();
		g_pCursor->init(m_pDefaultCamera);

		UnitSelector* pSelector = pCursorObj->addComponent<UnitSelector>();
		pSelector->init(g_pCursor, g_TeamID1);
	}

	//{
	//	std::string filePath = "Resources/Hoge.json";
	//	GameDevice::getTextureManager().load("Hoge", L"Resources/Hoge.png");

	//	BlockbenchLoader loader;
	//	loader.load(filePath, "Hoge", "Hoge");
	//	auto& matrices = loader.getModel("Hoge")->getCubeMatrices();

	//	std::vector<UnitInstanceInfo> instances;
	//	for (auto& matrix : matrices)
	//	{
	//		instances.emplace_back();
	//		auto& instance = instances.back();

	//		DirectX::XMStoreFloat4x4(&instance.instanceMat, DirectX::XMMatrixTranspose(matrix));
	//		instance.instanceColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	//	}

	//	auto& uvOrigins = loader.getModel("Hoge")->getUVOrigins();
	//	auto& uvSizes = loader.getModel("Hoge")->getUVSizes();
	//	for (int i = 0; i < (int)instances.size(); i++)
	//	{
	//		for (int j = 0; j < BlockbenchModel::cubeFaceCount / 2; j++)
	//		{
	//			//原点をセット
	//			instances[i].instanceUVOrigins.m[j][0] = uvOrigins.at(j).x;
	//			instances[i].instanceUVOrigins.m[j][1] = uvOrigins.at(j).y;
	//			instances[i].instanceUVOrigins.m[j][2] = uvOrigins.at(j + 3).x;
	//			instances[i].instanceUVOrigins.m[j][3] = uvOrigins.at(j + 3).y;

	//			//サイズをセット
	//			instances[i].instanceUVSizes.m[j][0] = uvSizes.at(j).x;
	//			instances[i].instanceUVSizes.m[j][1] = uvSizes.at(j).y;
	//			instances[i].instanceUVSizes.m[j][2] = uvSizes.at(j + 3).x;
	//			instances[i].instanceUVSizes.m[j][3] = uvSizes.at(j + 3).y;
	//		}
	//	}

	//	m_pBBModelMaterial->setMainTexture(GameDevice::getTextureManager().getTexture("Hoge"));

	//	auto pModelObj = ModelGameObjectHelper::instantiateModel<UnitInstanceInfo>(this, GameDevice::getModelManager().getModel("Cube"), true);
	//	auto pInstancedRenderer = pModelObj->getChildren().at(0)->getComponent<InstancedRenderer<UnitInstanceInfo>>();
	//	pInstancedRenderer->setMaterial(m_pBBModelMaterial);
	//	pInstancedRenderer->setInstanceInfo(instances);
	//	loader.unLoadModels();
	//}
}

void GameScene::update()
{
	if (GameDevice::getInput().isKeyDown(DIK_1))
	{
		pPlayer1->setActive(true);
	}

	m_pHealthSlider->setCurrentValue(m_pUnit1->getHealth());
}

void GameScene::shutdown()
{
	//マテリアルの削除
	delete m_pInstancingMaterial;
	delete m_pValueMapMaterial;
	delete m_pBBModelMaterial;
}

void GameScene::lateUpdate()
{
	//情報マップをクリア
	m_ValueMap1.clearAll();
	m_ValueMap2.clearAll();
}
