#include "SelectScene.h"
#include "Device\GameDevice.h"
#include "Scene\GameScene.h"
#include "UI\UIStageList.h"

SelectScene::SelectScene(GameScene* pGameScene)
	: m_pGameScene(pGameScene)
{
}

std::string SelectScene::nextScene()
{
	return "GameScene";
}

bool SelectScene::isEnd()
{
	return GameDevice::getInput().isKeyDown(DIK_SPACE);
}

void SelectScene::start()
{
	auto pObj = new GameObject(this);
	m_pUIStageList = pObj->addComponent<UIStageList>();
}

void SelectScene::update()
{
}

void SelectScene::shutdown()
{
	//セレクトされているステージを渡す
	m_pGameScene->setStage(m_pUIStageList->getSelectedStageName());
}
