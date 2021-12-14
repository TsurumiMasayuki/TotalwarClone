#include "TotalwarCloneGame.h"

#include <filesystem>

#include "Scene\TitleScene.h"
#include "Scene\SelectScene.h"
#include "Scene\GameScene.h"
#include "Stage\Stage.h"
#include "Utility\JsonFileManager.h"

TotalwarCloneGame::TotalwarCloneGame(HINSTANCE& hinstance, HWND& hwnd)
	: AbstractGame(hinstance, hwnd)
{
}

void TotalwarCloneGame::onStart()
{
	GameDevice::getTextureManager().load("BoxFill", L"Resources/BoxFill.png");
	GameDevice::getTextureManager().load("CircleFill", L"Resources/CircleFill.png");
	GameDevice::getTextureManager().load("CombatIcon", L"Resources/Textures/CombatIcon.png");
	GameDevice::getTextureManager().load("Energy", L"Resources/Textures/Energy.png");

	GameDevice::getModelManager().startLoading();
	GameDevice::getModelManager().load("Sphere", "Resources/Models/SpherePBR/", "SpherePBR");
	GameDevice::getModelManager().load("Cube", "Resources/Models/cube/", "cube");

	GameDevice::getAudioManager().loadWavFile("Shot1", L"Resources/Audio/Shot1.wav");

	auto pGameScene = new GameScene();

	m_SceneManager.addScene("TitleScene", new TitleScene());
	m_SceneManager.addScene("SelectScene", new SelectScene(pGameScene));
	m_SceneManager.addScene("GameScene", pGameScene);
	m_SceneManager.changeScene("SelectScene");

	//�U���X�e�[�^�X�̓ǂݍ���
	{
		auto& attackStatsManager = JsonFileManager<AttackStats>::getInstance();
		attackStatsManager.load("TestAttack", "Resources/AttackStats/TestAttack.json");
		attackStatsManager.load("TestAttack_Strong", "Resources/AttackStats/TestAttack_Strong.json");
		attackStatsManager.load("TestAttack_Snipe", "Resources/AttackStats/TestAttack_Snipe.json");
		attackStatsManager.load("CruiserNormalAttack", "Resources/AttackStats/CruiserNormalAttack.json");
	}

	//���j�b�g�X�e�[�^�X�̓ǂݍ���
	{
		auto& unitStatsManager = JsonFileManager<UnitStats>::getInstance();
		unitStatsManager.load("NormalCorvette", "Resources/UnitStats/NormalCorvette.json");
		unitStatsManager.load("NormalCruiser", "Resources/UnitStats/NormalCruiser.json");
		unitStatsManager.load("NormalBattleship", "Resources/UnitStats/NormalBattleship.json");
		unitStatsManager.load("SniperCruiser", "Resources/UnitStats/SniperCruiser.json");
		unitStatsManager.load("SuperBattleship", "Resources/UnitStats/SuperBattleship.json");
	}

	//�X�e�[�W�̓ǂݍ���
	{
		auto& stageManager = JsonFileManager<Stage>::getInstance();

		for (auto& file : std::filesystem::directory_iterator("Resources/Stages"))
		{
			//�p�X���擾
			std::filesystem::path filename = file.path().filename();
			//�g���q���폜
			filename.replace_extension();
			//�t�@�C�������[�h
			stageManager.load(filename.string(), file.path().string());
		}
	}
}

void TotalwarCloneGame::onUpdate()
{
}

void TotalwarCloneGame::onShutdown()
{
	GameDevice::getModelManager().endLoading();
}