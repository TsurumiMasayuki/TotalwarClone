#pragma once
#include <vector>
#include "Scene\Base\AbstractScene.h"
#include "Utility\Timer.h"

namespace Action
{
	class ActionManager;
}

class SpriteRenderer;
class D2DTextRenderer;

class TitleScene
	: public AbstractScene
{
public:
	virtual std::string nextScene() override;
	virtual bool isEnd() override;
	virtual void start() override;
	virtual void update() override;
	virtual void shutdown() override;

private:
	std::vector<SpriteRenderer*> m_SpriteRenderers;
	std::vector<Action::ActionManager*> m_ActionManagers;
	D2DTextRenderer* m_pTextRenderer;
	bool m_IsSceneChangeBegin;
	Timer m_SceneChangeTimer;
};