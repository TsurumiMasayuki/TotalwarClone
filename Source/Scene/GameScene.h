#pragma once
#include "AI\ValueMap.h"
#include "Scene\Base\AbstractScene.h"

class GameObject;
class Unit;
class InstancingMaterial;
class BBModelMaterial;
class ValueMapMaterial;
class Model;
class UISlider;

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
	BBModelMaterial* m_pBBModelMaterial;
	ValueMapMaterial* m_pValueMapMaterial;

	Model* m_pBBCube;

	Unit* m_pUnit1;
	UISlider* m_pHealthSlider;
	UISlider* m_pShieldSlider;
};