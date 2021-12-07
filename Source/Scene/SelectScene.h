#pragma once
#include "Scene\Base\AbstractScene.h"

class GameScene;
class UIStageList;

class SpriteRenderer;

namespace Action
{
	class ActionManager;
}

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
	std::vector<SpriteRenderer*> m_SpriteRenderers;
	std::vector<Action::ActionManager*> m_ActionManagers;

	GameScene* m_pGameScene;
	UIStageList* m_pUIStageList;

	bool m_IsChangeScene;
	Action::ActionManager* m_pActionManager;
};