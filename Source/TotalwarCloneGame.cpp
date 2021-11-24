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

	GameDevice::getModelManager().startLoading();
	GameDevice::getModelManager().load("Sphere", "Resources/Models/SpherePBR/", "SpherePBR");
	GameDevice::getModelManager().load("Cube", "Resources/Models/cube/", "cube");

	GameDevice::getAudioManager().loadWavFile("Shot1", L"Resources/Audio/Shot1.wav");

	auto pGameScene = new GameScene();

	m_SceneManager.addScene("TitleScene", new TitleScene());
	m_SceneManager.addScene("SelectScene", new SelectScene(pGameScene));
	m_SceneManager.addScene("GameScene", pGameScene);
	m_SceneManager.changeScene("TitleScene");

	//ステージの読み込み
	{
		auto& stageManager = JsonFileManager<Stage>::getInstance();
		//stageManager.load("TestStage", "Resources/Stages/TestStage.json");

		for (auto& file : std::filesystem::directory_iterator("Resources/Stages"))
		{
			//パスを取得
			std::filesystem::path filename = file.path().filename();
			filename.replace_extension();
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