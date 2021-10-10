#pragma once
#include "AI\ValueMap.h"
#include "Scene\Base\AbstractScene.h"

class GameObject;
class InstancingMaterial;

class GameScene
	: public AbstractScene
{
public:
	virtual std::string nextScene() override;
	virtual bool isEnd() override;
	virtual void start() override;
	virtual void update() override;
	virtual void shutdown() override;

	void lateUpdate();

private:
	ValueMap m_ValueMap1;
	ValueMap m_ValueMap2;

	InstancingMaterial* m_pInstancingMaterial;
	InstancingMaterial* m_pValueMapMaterial;
};