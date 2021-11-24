#pragma once
#include "Scene\Base\AbstractScene.h"

class GameScene;
class UIStageList;

class SelectScene
	: public AbstractScene
{
public:
	SelectScene(GameScene* pGameScene);

	virtual std::string nextScene() override;
	virtual bool isEnd() override;
	virtual void start() override;
	virtual void update() override;
	virtual void shutdown() override;

private:
	GameScene* m_pGameScene;
	UIStageList* m_pUIStageList;
};