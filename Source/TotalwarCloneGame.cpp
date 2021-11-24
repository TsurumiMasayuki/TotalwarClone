#include "TotalwarCloneGame.h"
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
		stageManager.load("TestStage", "Resources/Stages/TestStage.json");
		stageManager.load("Stage1", "Resources/Stages/Stage1.json");
	}
}

void TotalwarCloneGame::onUpdate()
{
}

void TotalwarCloneGame::onShutdown()
{
	GameDevice::getModelManager().endLoading();
}