#pragma once
#include <vector>
#include <unordered_map>
#include "AI\ValueMap.h"
#include "Scene\Base\AbstractScene.h"
#include "Effect\EffectRenderHelper.h"
#include "Utility\Timer.h"

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

class GUISpriteRenderer;
class D2DTextRenderer;

class GameScene
	: public AbstractScene
{
public:
	virtual std::string nextScene() override;
	virtual bool isEnd() override;
	virtual void start() override;
	virtual void update() override;
	virtual void shutdown() override;

	virtual void lateUpdate() override;

	void setStage(const std::string& stageName);

private:
	ValueMap m_ValueMap1;
	ValueMap m_ValueMap2;

	InstancingMaterial* m_pInstancingMaterial;
	BBModelMaterial* m_pBBModelMaterial;
	ValueMapMaterial* m_pValueMapMaterial;

	Model* m_pBBCube;

	std::unordered_map<std::string, UnitRenderHelper*> m_UnitRenderHelpers;
	EffectRenderHelper* m_pEffectRenderHelper;
	UnitSelector* m_pUnitSelector;

	Player* m_pPlayer;
	AIPlayer* m_pAIPlayer;

	Vec3 m_PreCameraPos;
	Vec3 m_PreCameraAngles;

	UIUnitList* m_pUIUnitList;
	UIUnitPlacer* m_pUIUnitPlacer;

	D2DTextRenderer* m_pWinLoseText;

	std::string m_StageName;

	//ÉVÅ[ÉìëJà⁄ån
	std::vector<GUISpriteRenderer*> m_SpriteRenderers;
	std::vector<Action::ActionManager*> m_ActionManagers;
	bool m_IsSceneChangeBegin;
	Timer m_SceneChangeTimer;
};