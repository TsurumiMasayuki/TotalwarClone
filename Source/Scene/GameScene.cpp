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

#include "Player\AIPlayer\AIPlayer.h"

#include "UnitInfo\UnitStats.h"

#include "Blockbench\BlockbenchModel.h"
#include "Blockbench\BlockbenchLoader.h"

Unit* g_pUnit1;
Unit* g_pUnit2;

UnitStats g_UnitStats1;
UnitStats g_UnitStats2;

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
	//マテリアルの生成
	m_pInstancingMaterial = new InstancingMaterial();
	m_pInstancingMaterial->init(DX12GraphicsCore::g_pDevice.Get());

	//マテリアルの生成
	m_pValueMapMaterial = new ValueMapMaterial();
	m_pValueMapMaterial->init(DX12GraphicsCore::g_pDevice.Get());

	//物理マネージャーの生成
	auto pPhysicsManagerObj = new GameObject(this);
	pPhysicsManagerObj->addComponent<PhysicsManagerB2>();

	g_UnitStats1 =
	{
		25,			//数
		Vec3(0.5f, 0.5f, 1.0f),	//オブジェクトのサイズ
		3.0f,		//ユニットごとの間隔
		100.0f,		//体力
		0.0f,		//シールド
		10.0f,		//速度
		10.0f,		//回転速度
	};

	g_UnitStats2 =
	{
		5,			//数
		Vec3(5.0f, 5.0f, 10.0f),	//オブジェクトのサイズ
		8.0f,		//ユニットごとの間隔
		100.0f,		//体力
		0.0f,		//シールド
		10.0f,		//速度
		10.0f,		//回転速度
	};

	auto pObj1 = ModelGameObjectHelper::instantiateModel<UnitInstanceInfo>(this, GameDevice::getModelManager().getModel("Sphere"), true);
	auto pObj2 = ModelGameObjectHelper::instantiateModel<UnitInstanceInfo>(this, GameDevice::getModelManager().getModel("Sphere"), true);
	auto pObj3 = ModelGameObjectHelper::instantiateModel<UnitInstanceInfo>(this, GameDevice::getModelManager().getModel("Sphere"), true);

	pObj1->getChildren().at(0)->getTransform().setLocalPosition(Vec3(-50.0f, 0.0f, 0.0f));
	pObj1->getChildren().at(0)->getComponent<InstancedRenderer<UnitInstanceInfo>>()->setMaterial(m_pInstancingMaterial);
	g_pUnit1 = pObj1->getChildren().at(0)->addComponent<Unit>();
	g_pUnit1->init(g_TeamID1, &g_UnitStats1, &m_ValueMap1);
	g_pUnit1->setPosition(Vec3(-50.0f, 0.0f, 0.0f), 90.0f, 10);

	pObj2->getChildren().at(0)->getTransform().setLocalPosition(Vec3(100.0f, 0.0f, 0.0f));
	pObj2->getChildren().at(0)->getComponent<InstancedRenderer<UnitInstanceInfo>>()->setMaterial(m_pInstancingMaterial);
	g_pUnit2 = pObj2->getChildren().at(0)->addComponent<Unit>();
	g_pUnit2->init(g_TeamID2, &g_UnitStats2, &m_ValueMap2);
	g_pUnit2->setPosition(Vec3(100.0f, 0.0f, 0.0f), -90.0f, 10);

	pObj3->getChildren().at(0)->getTransform().setLocalPosition(Vec3(50.0f, 0.0f, 0.0f));
	pObj3->getChildren().at(0)->getComponent<InstancedRenderer<UnitInstanceInfo>>()->setMaterial(m_pInstancingMaterial);
	auto pUnit3 = pObj3->getChildren().at(0)->addComponent<Unit>();
	pUnit3->init(g_TeamID2, &g_UnitStats1, &m_ValueMap2);
	pUnit3->setPosition(Vec3(50.0f, 0.0f, 0.0f), -90.0f, 10);

	//AIプレイヤー1の生成
	auto pPlayer1Obj = new GameObject(this);
	pPlayer1 = pPlayer1Obj->addComponent<AIPlayer>();
	pPlayer1->setActive(false);

	//AIプレイヤー2の生成
	auto pPlayer2Obj = new GameObject(this);
	auto pPlayer2 = pPlayer2Obj->addComponent<AIPlayer>();
	pPlayer2->setActive(false);

	pPlayer1->init(g_TeamID1, pPlayer2, &m_ValueMap2);
	pPlayer1->addUnit(g_pUnit1);
	
	pPlayer2->init(1, pPlayer1, &m_ValueMap1);
	pPlayer2->addUnit(g_pUnit2);
	pPlayer2->addUnit(pUnit3);

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

	{
		std::string filePath = "Resources/Hoge.json";

		BlockbenchLoader loader;
		loader.loadModel(filePath, "Hoge");
		auto& matrices = loader.getModel("Hoge")->getCubeMatrices();

		std::vector<UnitInstanceInfo> instances;
		for (auto& matrix : matrices)
		{
			instances.emplace_back();
			auto& instance = instances.back();

			DirectX::XMStoreFloat4x4(&instance.instanceMat, DirectX::XMMatrixTranspose(matrix));
			instance.instanceColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		}

		auto pModelObj = ModelGameObjectHelper::instantiateModel<UnitInstanceInfo>(this, GameDevice::getModelManager().getModel("Cube"), true);
		auto pInstancedRenderer = pModelObj->getChildren().at(0)->getComponent<InstancedRenderer<UnitInstanceInfo>>();
		pInstancedRenderer->setMaterial(m_pInstancingMaterial);
		pInstancedRenderer->setInstanceInfo(instances);
		loader.unLoadModels();
	}
}

void GameScene::update()
{
	if (GameDevice::getInput().isKeyDown(DIK_1))
	{
		pPlayer1->setActive(true);
	}

	if (GameDevice::getInput().isKeyDown(DIK_2))
	{
		g_pUnit1->setTarget(g_pUnit2);
	}
}

void GameScene::shutdown()
{
	//マテリアルの削除
	delete m_pInstancingMaterial;
	delete m_pValueMapMaterial;
}

void GameScene::lateUpdate()
{
	//情報マップをクリア
	m_ValueMap1.clearAll();
	m_ValueMap2.clearAll();
}
