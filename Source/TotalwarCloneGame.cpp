#include "TotalwarCloneGame.h"
#include "Scene\GameScene.h"

GameScene* g_pGameScene;

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

	g_pGameScene = new GameScene();

	m_SceneManager.addScene("GameScene", g_pGameScene);
	m_SceneManager.changeScene("GameScene");
}

void TotalwarCloneGame::onUpdate()
{
	g_pGameScene->lateUpdate();
}

void TotalwarCloneGame::onShutdown()
{
	GameDevice::getModelManager().endLoading();
}