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

	//スカイボックス用
	GameDevice::getTextureManager().load("SkyboxFront", L"Resources/Textures/Skybox/Front_1K_TEX.png");
	GameDevice::getTextureManager().load("SkyboxBack", L"Resources/Textures/Skybox/Back_1K_TEX.png");
	GameDevice::getTextureManager().load("SkyboxRight", L"Resources/Textures/Skybox/Right_1K_TEX.png");
	GameDevice::getTextureManager().load("SkyboxLeft", L"Resources/Textures/Skybox/Left_1K_TEX.png");
	GameDevice::getTextureManager().load("SkyboxUp", L"Resources/Textures/Skybox/Up_1K_TEX.png");
	GameDevice::getTextureManager().load("SkyboxDown", L"Resources/Textures/Skybox/Down_1K_TEX.png");

	GameDevice::getModelManager().startLoading();
	GameDevice::getModelManager().load("Sphere", "Resources/Models/SpherePBR/", "SpherePBR");
	GameDevice::getModelManager().load("Cube", "Resources/Models/cube/", "cube");

	GameDevice::getAudioManager().loadWavFile("Shot1", L"Resources/Audio/Shot1.wav");

	auto pGameScene = new GameScene();

	m_SceneManager.addScene("TitleScene", new TitleScene());
	m_SceneManager.addScene("SelectScene", new SelectScene(pGameScene));
	m_SceneManager.addScene("GameScene", pGameScene);
	m_SceneManager.changeScene("TitleScene");

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
		unitStatsManager.load("SuperBattleship", "Resources/UnitStats/SuperBattleship.json");
	}

	//ステージの読み込み
	{
		auto& stageManager = JsonFileManager<Stage>::getInstance();

		for (auto& file : std::filesystem::directory_iterator("Resources/Stages"))
		{
			//パスを取得
			std::filesystem::path filename = file.path().filename();
			//拡張子を削除
			filename.replace_extension();
			//ファイルをロード
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