#include "GameScene.h"
#include "Component\Graphics\InstancedRenderer.h"
#include "Component\Physics\BoxColliderBt.h"
#include "Device\GameDevice.h"
#include "Utility\ModelGameObjectHelper.h"
#include "Math\MathUtility.h"

#include "AI\ValueMap.h"
#include "AI\ValueMapRenderer.h"

#include "Component\Prototype\Unit.h"
#include "Component\Box2D\PhysicsManagerB2.h"

#include "Graphics\DX12\Material\DefaultMaterials.h"

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
	//�}�e���A���̐���
	m_pInstancingMaterial = new InstancingMaterial();
	m_pInstancingMaterial->init(DX12GraphicsCore::g_pDevice.Get());

	//�}�e���A���̐���
	m_pValueMapMaterial = new InstancingMaterial();
	m_pValueMapMaterial->init(DX12GraphicsCore::g_pDevice.Get());

	//�����}�l�[�W���[�̐���
	auto pPhysicsManagerObj = new GameObject(this);
	pPhysicsManagerObj->addComponent<PhysicsManagerB2>();

	g_UnitStats1 =
	{
		100.0f,	//HP
		30,		//�A�[�}�[
		25,		//�ߐږ����m��
		30,		//�ߐږh��m��
		25.0f,	//�ߐڃ_���[�W
		1.0f	//�U���Ԋu
	};

	g_UnitStats2 =
	{
		100.0f,	//HP
		30,		//�A�[�}�[
		25,		//�ߐږ����m��
		30,		//�ߐږh��m��
		25.0f,	//�ߐڃ_���[�W
		1.0f	//�U���Ԋu
	};

	auto pObj1 = ModelGameObjectHelper::instantiateModel<UnitInstanceInfo>(this, GameDevice::getModelManager().getModel("Sphere"), true);
	auto pObj2 = ModelGameObjectHelper::instantiateModel<UnitInstanceInfo>(this, GameDevice::getModelManager().getModel("Sphere"), true);

	pObj1->getChildren().at(0)->getTransform().setLocalPosition(Vec3(-50.0f, 0.0f, 0.0f));
	pObj1->getChildren().at(0)->getComponent<InstancedRenderer<UnitInstanceInfo>>()->setMaterial(m_pInstancingMaterial);
	g_pUnit1 = pObj1->getChildren().at(0)->addComponent<Unit>();
	g_pUnit1->init(2, 3.0f, 0, &g_UnitStats1, &m_ValueMap);
	g_pUnit1->setPosition(Vec3(-50.0f, 0.0f, 0.0f), 90.0f);

	pObj2->getChildren().at(0)->getTransform().setLocalPosition(Vec3(50.0f, 0.0f, 0.0f));
	pObj2->getChildren().at(0)->getComponent<InstancedRenderer<UnitInstanceInfo>>()->setMaterial(m_pInstancingMaterial);
	g_pUnit2 = pObj2->getChildren().at(0)->addComponent<Unit>();
	g_pUnit2->init(2, 3.0f, 1, &g_UnitStats2, &m_ValueMap);
	g_pUnit2->setPosition(Vec3(50.0f, 0.0f, 0.0f), -90.0f);

	//���}�b�v�`��̐���
	auto pValueMapRendererObj = new GameObject(this);
	auto pValueMapRenderer = pValueMapRendererObj->addComponent<ValueMapRenderer<ValueTypes::Health>>();
	pValueMapRenderer->init(&m_ValueMap, m_pValueMapMaterial, Color(DirectX::Colors::LightGreen));
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
	//�}�e���A���̍폜
	delete m_pInstancingMaterial;
}

void GameScene::lateUpdate()
{
	//���}�b�v���N���A
	m_ValueMap.clearAll();
}
