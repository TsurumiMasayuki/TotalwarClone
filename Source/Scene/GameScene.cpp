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

#include "Player\Player.h"
#include "Player\AIPlayer\AIPlayer.h"

#include "Unit\UnitStats.h"
#include "Utility\JsonFileManager.h"

#include "UI\UIUnitList.h"

#include "Blockbench\BlockbenchModel.h"
#include "Blockbench\BlockbenchLoader.h"

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
	//�U���X�e�[�^�X�̓ǂݍ���
	{
		auto& attackStatsManager = JsonFileManager<AttackStats>::getInstance();
		attackStatsManager.load("TestAttack", "Resources/AttackStats/TestAttack.json");
		attackStatsManager.load("TestAttack_Strong", "Resources/AttackStats/TestAttack_Strong.json");
		attackStatsManager.load("TestAttack_Snipe", "Resources/AttackStats/TestAttack_Snipe.json");
	}

	//���j�b�g�X�e�[�^�X�̓ǂݍ���
	{
		auto& unitStatsManager = JsonFileManager<UnitStats>::getInstance();
		unitStatsManager.load("NormalCorvette", "Resources/UnitStats/NormalCorvette.json");
		unitStatsManager.load("NormalBattleship", "Resources/UnitStats/NormalBattleship.json");
		unitStatsManager.load("SniperCruiser", "Resources/UnitStats/SniperCruiser.json");
	}

	//�}�e���A���̐���
	m_pInstancingMaterial = new InstancingMaterial();
	m_pInstancingMaterial->init(DX12GraphicsCore::g_pDevice.Get());

	//�}�e���A���̐���
	m_pValueMapMaterial = new ValueMapMaterial();
	m_pValueMapMaterial->init(DX12GraphicsCore::g_pDevice.Get());

	//�}�e���A���̐���
	m_pBBModelMaterial = new BBModelMaterial();
	m_pBBModelMaterial->init(DX12GraphicsCore::g_pDevice.Get());

	//�����}�l�[�W���[�̐���
	auto pPhysicsManagerObj = new GameObject(this);
	pPhysicsManagerObj->addComponent<PhysicsManagerB2>();

	//�J�[�\���p�I�u�W�F�N�g
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

	//AI�v���C���[1�̐���
	auto pPlayer1Obj = new GameObject(this);
	pPlayer1 = pPlayer1Obj->addComponent<Player>();

	//AI�v���C���[2�̐���
	auto pPlayer2Obj = new GameObject(this);
	auto pPlayer2 = pPlayer2Obj->addComponent<AIPlayer>();

	pPlayer1->init(g_TeamID1, pPlayer2, &m_ValueMap2);
	pPlayer2->init(g_TeamID2, pPlayer1, &m_ValueMap1);

	{
		//�����炢�̂��ʐ���
		for (int i = -2; i < 3; i++)
		{
			auto pUnitObj = ModelGameObjectHelper::instantiateModel<UnitInstanceInfo>(this, pModel, true);
			pUnitObj->getChildren().at(0)->getComponent<InstancedRenderer<UnitInstanceInfo>>()->setMaterial(m_pInstancingMaterial);
			auto pUnit = pUnitObj->getChildren().at(0)->addComponent<Unit>();
			pUnit->init(g_TeamID1, pUnitStats1, &m_ValueMap1);
			pUnit->setPosition(Vec3(200.0f * i, 0.0f, 0.0f), 0.0f, 10);

			pPlayer1->addUnit(pUnit);
		}

		auto pUIObj = new GameObject(this);
		pUIObj->setParent(&m_pDefaultCamera->getUser());
		UIUnitList* pUnitList = pUIObj->addComponent<UIUnitList>();
		pUnitList->init(pPlayer1, pSelector, 8.0f);
	}

	{
		//�����炢�̂��ʐ���
		for (int i = -2; i < 3; i++)
		{
			auto pUnitObj = ModelGameObjectHelper::instantiateModel<UnitInstanceInfo>(this, pModel, true);
			pUnitObj->getChildren().at(0)->getComponent<InstancedRenderer<UnitInstanceInfo>>()->setMaterial(m_pInstancingMaterial);
			auto pUnit = pUnitObj->getChildren().at(0)->addComponent<Unit>();
			pUnit->init(g_TeamID2, pUnitStats1, &m_ValueMap2);
			pUnit->setPosition(Vec3(200.0f * i, 0.0f, 200.0f), 180.0f, 10);

			pPlayer2->addUnit(pUnit);
		}
	}

	//���}�b�v�`��̐���
	auto pValueMapRendererObj = new GameObject(this);
	auto pValueMapRenderer = pValueMapRendererObj->addComponent<ValueMapRenderer<UnitStatsValues::Health>>();
	pValueMapRenderer->init(&m_ValueMap2, m_pValueMapMaterial, Color(DirectX::Colors::LightGreen));

	//���C�L���X�g����p���ʃI�u�W�F�N�g
	{
		auto pPlaneObj = new GameObject(this);
		pPlaneObj->getTransform().setLocalScale(Vec3(3000.0f, 0.1f, 3000.0f));
		auto pCollider = pPlaneObj->addComponent<BoxColiiderBt>();
		pCollider->setMass(0.0f);
	}

	{
	//	std::string filePath = "Resources/TextureTest.json";
	//	GameDevice::getTextureManager().load("TextureTest", L"Resources/TextureTest.png");

	//	BlockbenchLoader loader;
	//	loader.load(filePath, "TextureTest", "TextureTest");
	//	auto& matrices = loader.getModel("TextureTest")->getCubeMatrices();

	//	std::vector<UnitInstanceInfo> instances;
	//	for (auto& matrix : matrices)
	//	{
	//		instances.emplace_back();
	//		auto& instance = instances.back();

	//		DirectX::XMStoreFloat4x4(&instance.instanceMat, DirectX::XMMatrixTranspose(matrix));
	//		instance.instanceColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	//	}

	//	auto& uvOrigins = loader.getModel("TextureTest")->getUVOrigins();
	//	auto& uvSizes = loader.getModel("TextureTest")->getUVSizes();
	//	for (int i = 0; i < (int)instances.size(); i++)
	//	{
	//		for (int j = 0; j < BlockbenchModel::cubeFaceCount / 2; j++)
	//		{
	//			//���_���Z�b�g
	//			instances[i].instanceUVOrigins.m[j][0] = uvOrigins.at(j).x;
	//			instances[i].instanceUVOrigins.m[j][1] = uvOrigins.at(j).y;
	//			instances[i].instanceUVOrigins.m[j][2] = uvOrigins.at(j + 3).x;
	//			instances[i].instanceUVOrigins.m[j][3] = uvOrigins.at(j + 3).y;

	//			//�T�C�Y���Z�b�g
	//			instances[i].instanceUVSizes.m[j][0] = uvSizes.at(j).x;
	//			instances[i].instanceUVSizes.m[j][1] = uvSizes.at(j).y;
	//			instances[i].instanceUVSizes.m[j][2] = uvSizes.at(j + 3).x;
	//			instances[i].instanceUVSizes.m[j][3] = uvSizes.at(j + 3).y;
	//		}
	//	}

	//	m_pBBModelMaterial->setMainTexture(GameDevice::getTextureManager().getTexture("TextureTest"));

	//	auto pModelObj = ModelGameObjectHelper::instantiateModel<UnitInstanceInfo>(this, GameDevice::getModelManager().getModel("Cube"), true);
	//	auto pInstancedRenderer = pModelObj->getChildren().at(0)->getComponent<InstancedRenderer<UnitInstanceInfo>>();
	//	pInstancedRenderer->setMaterial(m_pBBModelMaterial);
	//	pInstancedRenderer->setInstanceInfo(instances);
	//	loader.unLoadModels();
	}
}

void GameScene::update()
{
	if (GameDevice::getInput().isKeyDown(DIK_1))
	{
		pPlayer1->setActive(true);
	}
}

void GameScene::shutdown()
{
	//�}�e���A���̍폜
	delete m_pInstancingMaterial;
	delete m_pValueMapMaterial;
	delete m_pBBModelMaterial;
}

void GameScene::lateUpdate()
{
	//���}�b�v���N���A
	m_ValueMap1.clearAll();
	m_ValueMap2.clearAll();
}
