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
	//�ǂ��炩���s�k������I��
	return m_SceneChangeTimer.isTime() && Game::g_GameState == Game::GameState::Result &&
		(m_pPlayer->isDefeat() || m_pAIPlayer->isDefeat());
}

void GameScene::start()
{
	//�̕�
	const float plateWidth = 64.0f;
	//�̍���
	const float plateHeight = 64.0f;

	//�̐�(��)
	const int plateCountX = (int)(WindowWidth / plateWidth);
	//�̐�(�c)
	const int plateCountY = (int)(WindowHeight / plateHeight) + 1;

	const float offsetX = -plateWidth * plateCountX / 2 + plateWidth * 0.5f;
	const float offsetY = -plateHeight * plateCountY / 2 + plateHeight * 0.5f;

	//�Q�[���J�n�����o
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

			//�����摜��ݒ�
			auto pRenderer = pObj->addComponent<GUISpriteRenderer>();
			pRenderer->setTextureByName("BoxFill");
			pRenderer->setColor(Color(0.0f, 0.0f, 0.0f, 1.0f));

			auto pActionManager = pObj->addComponent<Action::ActionManager>();

			//�C�[�W���O
			float ease = 1.0f - Easing::easeInQuad((float)x / (float)plateCountX);
			pActionManager->enqueueAction(
				new Action::WaitForSeconds(3.0f * ease)
			);

			//�����摜�𔒂����Ȃ������
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

	//����ׂ�
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

	//SEManager�̐ݒ�
	SEManager::getInstance().setListner(&m_pDefaultCamera->getTransform());

	Game::g_GameState = Game::GameState::PreparePhase;

	//�}�e���A���̐���
	m_pInstancingMaterial = new InstancingMaterial();
	m_pInstancingMaterial->init(DX12GraphicsCore::g_pDevice.Get());

	//�}�e���A���̐���
	m_pValueMapMaterial = new ValueMapMaterial();
	m_pValueMapMaterial->init(DX12GraphicsCore::g_pDevice.Get());
	m_pValueMapMaterial->setMainTexture(GameDevice::getTextureManager().getTexture("CircleFill"));

	//�}�e���A���̐���
	m_pBBModelMaterial = new BBModelMaterial();
	m_pBBModelMaterial->init(DX12GraphicsCore::g_pDevice.Get());

	//�����}�l�[�W���[�̐���
	auto pPhysicsManagerObj = new GameObject(this);
	pPhysicsManagerObj->addComponent<PhysicsManagerB2>();

	//�J�[�\���p�I�u�W�F�N�g
	auto pCursorObj = ModelGameObjectHelper::instantiateModel<int>(this, GameDevice::getModelManager().getModel("Sphere"));
	pCursorObj->getTransform().setLocalScale(Vec3(1.0f));

	auto pCursor = pCursorObj->addComponent<Cursor>();
	pCursor->init(m_pDefaultCamera);

	m_pUnitSelector = pCursorObj->addComponent<UnitSelector>();
	m_pUnitSelector->init(pCursor, g_TeamID1, m_pValueMapMaterial);

	//AI�v���C���[1�̐���
	auto pPlayer1Obj = new GameObject(this);
	m_pPlayer = pPlayer1Obj->addComponent<Player>();

	//AI�v���C���[2�̐���
	auto pPlayer2Obj = new GameObject(this);
	m_pAIPlayer = pPlayer2Obj->addComponent<AIPlayer>();

	m_pPlayer->init(g_TeamID1, m_pAIPlayer, &m_ValueMap2);
	m_pAIPlayer->init(g_TeamID2, m_pPlayer, &m_ValueMap1);

	auto pSphereModel = GameDevice::getModelManager().getModel("Sphere");

	//���j�b�g�`��p�I�u�W�F�N�g����
	for (const auto& pair : JsonFileManager<UnitStats>::getInstance().getAll())
	{
		auto pRendererObj = ModelGameObjectHelper::instantiateModel<UnitInstanceInfo>(this, pSphereModel, true);
		auto pRenderer = pRendererObj->getChildren().at(0)->getComponent<InstancedRenderer<UnitInstanceInfo>>();
		pRenderer->setMaterial(m_pInstancingMaterial);

		m_UnitRenderHelpers.emplace(pair.first, new UnitRenderHelper(&pair.second, nullptr, pRenderer));
	}
	
	auto pCubeModel = GameDevice::getModelManager().getModel("Cube");

	//�G�t�F�N�g�`��p�I�u�W�F�N�g����
	{
		auto pRendererObj = ModelGameObjectHelper::instantiateModel<EffectInstanceInfo>(this, pCubeModel, true);
		auto pRenderer = pRendererObj->getChildren().at(0)->getComponent<InstancedRenderer<EffectInstanceInfo>>();
		pRenderer->setMaterial(m_pInstancingMaterial);

		m_pEffectRenderHelper = new EffectRenderHelper(pRenderer);
	}

	//UI����
	{
		auto pUIObj1 = new GameObject(this);
		pUIObj1->setParent(&m_pDefaultCamera->getUser());
		m_pUIUnitPlacer = pUIObj1->addComponent<UIUnitPlacer>();
		m_pUIUnitPlacer->init(pCursor, m_pPlayer, m_pUnitSelector, &m_ValueMap1, &m_UnitRenderHelpers, m_pEffectRenderHelper);

		auto pUIObj2 = new GameObject(this);
		pUIObj2->setParent(&m_pDefaultCamera->getUser());
		m_pUIUnitList = pUIObj2->addComponent<UIUnitList>();
		pUIObj2->setActive(false);
	}

	//�X�e�[�W�ݒ�ǂݍ���
	const Stage& stage = JsonFileManager<Stage>::getInstance().get(m_StageName);
	m_pPlayer->setEnergy(stage.m_PlayerEnergy);

	//���j�b�g����
	for (const auto& enemy : stage.getUnitList())
	{
		const UnitStats& unitStats = JsonFileManager<UnitStats>::getInstance().get(enemy.m_Name);

		GameObject* pUnitObj = new GameObject(this);
		auto pUnit = pUnitObj->addComponent<Unit>();
		pUnit->init(m_pAIPlayer, &unitStats, &m_ValueMap2, m_UnitRenderHelpers.at(unitStats.m_Name), m_pEffectRenderHelper, false);
		pUnit->setPosition(enemy.m_Position, enemy.m_Angle, enemy.m_Width);

		m_pAIPlayer->addUnit(pUnit);
	}

	//���}�b�v�`��̐���
	//auto pValueMapRendererObj = new GameObject(this);
	//auto pValueMapRenderer = pValueMapRendererObj->addComponent<ValueMapRenderer<UnitStatsValues::Health>>();
	//pValueMapRenderer->init(&m_ValueMap2, m_pValueMapMaterial, Color(DirectX::Colors::LightGreen));

	//���C�L���X�g����p���ʃI�u�W�F�N�g
	{
		auto pPlaneObj = new GameObject(this);
		pPlaneObj->getTransform().setLocalScale(Vec3(3000.0f, 0.1f, 3000.0f));
		auto pCollider = pPlaneObj->addComponent<BoxColiiderBt>();
		pCollider->setMass(0.0f);
	}

	//���s�e�L�X�g
	{
		auto pTextObj = new GameObject(this);

		//TextRenderer�̐ݒ�
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

	//��͒l�\��
	{
		auto pUIObj = new GameObject(this);
		pUIObj->setParent(&m_pDefaultCamera->getUser());

		auto pUIBattleSlider = pUIObj->addComponent<UIBattleSlider>();
		pUIBattleSlider->init(m_pPlayer, m_pAIPlayer);
	}

	{
	//	GameDevice::getModelManager().load("BBCube", "Resources/Models/BBCube", "BBCube");

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

	//	auto pModel = GameDevice::getModelManager().getModel("BBCube");
	//	auto pModelObj = ModelGameObjectHelper::instantiateModel<UnitInstanceInfo>(this, pModel, true);
	//	auto pInstancedRenderer = pModelObj->getChildren().at(0)->getComponent<InstancedRenderer<UnitInstanceInfo>>();
	//	pInstancedRenderer->setMaterial(m_pBBModelMaterial);
	//	pInstancedRenderer->setInstanceInfo(instances);
	//	loader.unLoadModels();
	}
	
	m_IsSceneChangeBegin = false;
	m_SceneChangeTimer.setMaxTime(5.5f);
	m_SceneChangeTimer.reset();
}

void GameScene::update()
{
	//SEManager�̍X�V
	SEManager::getInstance().update();

	//CombatPhaseBegin��1�t���[���ŏI��点��
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

	//�퓬�t�F�[�Y���łǂ��炩����������X�e�[�g�ύX
	if (Game::g_GameState == Game::GameState::CombatPhase &&
		(m_pPlayer->isDefeat() || m_pAIPlayer->isDefeat()))
	{
		Game::g_GameState = Game::GameState::Result;
	}

	if (Game::g_GameState == Game::GameState::Result)
	{
		//�v���C���[�̏��s�ɂ���ăe�L�X�g�\��
		m_pWinLoseText->setText(!m_pPlayer->isDefeat() ? L"Win!" : L"Lose...");
	}

	//�X�y�[�X�L�[�������ꂽ��J�����ړ�
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

	//�V�[���I�����o
	if (GameDevice::getInput().isKeyDown(DIK_2) &&
		Game::g_GameState == Game::GameState::Result &&
		!m_IsSceneChangeBegin)
	{
		m_IsSceneChangeBegin = true;

		//���ԎZ�o�p�̍ő勗��
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
								new Action::ColorTo(Color(0.0f, 0.0f, 0.0f, 1.0f), m_SpriteRenderers.at(i), 0.5f),	//��������
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
	//�}�e���A���̍폜
	delete m_pInstancingMaterial;
	delete m_pValueMapMaterial;
	delete m_pBBModelMaterial;

	for (auto& pair : m_UnitRenderHelpers)
	{
		delete pair.second;
	}

	m_UnitRenderHelpers.clear();

	delete m_pEffectRenderHelper;

	SEManager::getInstance().clear();

	Game::g_GameState = Game::GameState::PreparePhase;
}

void GameScene::lateUpdate()
{
	//���}�b�v���N���A
	m_ValueMap1.clearAll();
	m_ValueMap2.clearAll();

	//InstancedRenderer�Ɉ�ē]��
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
