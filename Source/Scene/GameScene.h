#pragma once
#include <unordered_map>
#include "AI\ValueMap.h"
#include "Scene\Base\AbstractScene.h"

class GameObject;

class Unit;
class UnitRenderHelper;
class UnitSelector;

class UIUnitPlacer;
class UIUnitList;

class InstancingMaterial;
class BBModelMaterial;
class ValueMapMaterial;

class Model;
class Player;
class AIPlayer;

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

	std::unordered_map<std::string, UnitRenderHelper*> m_UnitRenderHelpers;
	UnitSelector* m_pUnitSelector;

	Player* m_pPlayer;
	AIPlayer* m_pAIPlayer;

	Vec3 m_PreCameraPos;
	Vec3 m_PreCameraAngles;

	UIUnitList* m_pUIUnitList;
	UIUnitPlacer* m_pUIUnitPlacer;
};