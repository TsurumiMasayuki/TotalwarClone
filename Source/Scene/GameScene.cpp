#include "GameScene.h"
#include "Component\Graphics\InstancedRenderer.h"
#include "Component\Physics\BoxColliderBt.h"
#include "Device\GameDevice.h"
#include "Utility\ModelGameObjectHelper.h"
#include "Math\MathUtility.h"

#include "AI\ValueMap.h"
#include "AI\ValueMapRenderer.h"

#include "Unit\Unit.h"
#include "Component\Box2D\PhysicsManagerB2.h"

#include "Graphics\DX12\Material\DefaultMaterials.h"

#include "Player\AIPlayer\AIPlayer.h"

#include "UnitInfo\UnitStats.h"

Unit* g_pUnit1;
Unit* g_pUnit2;

UnitStats g_UnitStats1;
UnitStats g_UnitStats2;

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
	m_pValueMapMaterial = new InstancingMaterial();
	m_pValueMapMaterial->init(DX12GraphicsCore::g_pDevice.Get());

	//物理マネージャーの生成
	auto pPhysicsManagerObj = new GameObject(this);
	pPhysicsManagerObj->addComponent<PhysicsManagerB2>();

	g_UnitStats1 =
	{
		100.0f,	//HP
		30,		//アーマー
		25,		//近接命中確率
		30,		//近接防御確率
		25.0f,	//近接ダメージ
		1.0f	//攻撃間隔
	};

	g_UnitStats2 =
	{
		50.0f,	//HP
		30,		//アーマー
		25,		//近接命中確率
		30,		//近接防御確率
		25.0f,	//近接ダメージ
		1.0f	//攻撃間隔
	};

	auto pObj1 = ModelGameObjectHelper::instantiateModel<UnitInstanceInfo>(this, GameDevice::getModelManager().getModel("Sphere"), true);
	auto pObj2 = ModelGameObjectHelper::instantiateModel<UnitInstanceInfo>(this, GameDevice::getModelManager().getModel("Sphere"), true);
	auto pObj3 = ModelGameObjectHelper::instantiateModel<UnitInstanceInfo>(this, GameDevice::getModelManager().getModel("Sphere"), true);

	pObj1->getChildren().at(0)->getTransform().setLocalPosition(Vec3(-50.0f, 0.0f, 0.0f));
	pObj1->getChildren().at(0)->getComponent<InstancedRenderer<UnitInstanceInfo>>()->setMaterial(m_pInstancingMaterial);
	g_pUnit1 = pObj1->getChildren().at(0)->addComponent<Unit>();
	g_pUnit1->init(25, 3.0f, 0, &g_UnitStats1, &m_ValueMap1);
	g_pUnit1->setPosition(Vec3(-50.0f, 0.0f, 0.0f), 90.0f);

	pObj2->getChildren().at(0)->getTransform().setLocalPosition(Vec3(100.0f, 0.0f, 0.0f));
	pObj2->getChildren().at(0)->getComponent<InstancedRenderer<UnitInstanceInfo>>()->setMaterial(m_pInstancingMaterial);
	g_pUnit2 = pObj2->getChildren().at(0)->addComponent<Unit>();
	g_pUnit2->init(25, 3.0f, 1, &g_UnitStats2, &m_ValueMap2);
	g_pUnit2->setPosition(Vec3(100.0f, 0.0f, 0.0f), -90.0f);

	pObj3->getChildren().at(0)->getTransform().setLocalPosition(Vec3(50.0f, 0.0f, 0.0f));
	pObj3->getChildren().at(0)->getComponent<InstancedRenderer<UnitInstanceInfo>>()->setMaterial(m_pInstancingMaterial);
	auto pUnit3 = pObj3->getChildren().at(0)->addComponent<Unit>();
	pUnit3->init(25, 3.0f, 1, &g_UnitStats1, &m_ValueMap2);
	pUnit3->setPosition(Vec3(50.0f, 0.0f, 0.0f), -90.0f);

	//AIプレイヤー1の生成
	auto pPlayer1Obj = new GameObject(this);
	auto pPlayer1 = pPlayer1Obj->addComponent<AIPlayer>();

	//AIプレイヤー2の生成
	auto pPlayer2Obj = new GameObject(this);
	auto pPlayer2 = pPlayer2Obj->addComponent<AIPlayer>();
	pPlayer2->setActive(false);

	pPlayer1->init(0, pPlayer2, &m_ValueMap2);
	pPlayer1->addUnit(g_pUnit1);
	
	pPlayer2->init(1, pPlayer1, &m_ValueMap1);
	pPlayer2->addUnit(g_pUnit2);
	pPlayer2->addUnit(pUnit3);

	//情報マップ描画の生成
	auto pValueMapRendererObj = new GameObject(this);
	auto pValueMapRenderer = pValueMapRendererObj->addComponent<ValueMapRenderer<UnitStatsValues::Health>>();
	pValueMapRenderer->init(&m_ValueMap2, m_pValueMapMaterial, Color(DirectX::Colors::LightGreen));
}

void GameScene::update()
{
	if (GameDevice::getInput().isKeyDown(DIK_1))
	{
		g_pUnit1->setDestination(Vec3(50.0f, 0.0f, 0.0f),  0.0f);
		g_pUnit2->setDestination(Vec3(-50.0f, 0.0f, 0.0f), 0.0f);
	}

	if (GameDevice::getInput().isKeyDown(DIK_2))
	{
		g_pUnit1->setTarget(g_pUnit2);
	}

	if (GameDevice::getInput().isKeyDown(DIK_3))
	{
		g_pUnit1->setDestination(Vec3(-50.0f, 0.0f, 0.0f), 90.0f);
		g_pUnit2->setDestination(Vec3(50.0f, 0.0f, 0.0f), -90.0f);
	}
}

void GameScene::shutdown()
{
	//マテリアルの削除
	delete m_pInstancingMaterial;
}

void GameScene::lateUpdate()
{
	//情報マップをクリア
	m_ValueMap1.clearAll();
	m_ValueMap2.clearAll();
}
