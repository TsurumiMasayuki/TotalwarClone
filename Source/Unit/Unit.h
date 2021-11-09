#pragma once
#include <vector>
#include <unordered_map>
#include "Component\Base\AbstractComponent.h"
#include "Utility\Timer.h"

#include "Blockbench/BlockbenchModel.h"
#include "Unit\ObjectPlacement.h"

struct UnitStats;
class ValueMap;
class UnitObject;
class UnitRenderHelper;

//ユニットクラス
class Unit
	: public AbstractComponent
{
public:
	virtual void onStart() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;

	virtual void onEnable() override;
	virtual void onDisable() override;

	void init(int teamID, const UnitStats* pUnitStats, ValueMap* pValueMap, UnitRenderHelper* pRenderHelper);
	void setPosition(const Vec3& position, float angle, int unitWidth);
	void setDestination(const Vec3& destination, float angle, int unitWidth, bool isMoveCommand = true);

	float getSpacePerObject() const;

	float getAngle() const;

	void setTarget(Unit* pTarget);
	Unit* getTarget();

	float getHealth() const;
	float getShield() const;
	int getObjectCount() const;
	const UnitStats* getUnitStats() const;

	int getTeamID() const;

	bool isStateLocked();

	void onEnterCombat(Unit* pEnemyUnit);
	bool isInCombat() const;

private:
	void updateCenterPosition();

private:
	std::vector<GameObject*> m_GameObjects;
	std::vector<UnitObject*> m_UnitObjects;

	Unit* m_pTargetUnit;

	int m_ObjectCount;
	int m_TeamID;

	Timer m_StateLockTimer;

	ValueMap* m_pValueMap;
	const UnitStats* m_pUnitStats;
	UnitRenderHelper* m_pRenderHelper;

	ObjectPlacement m_ObjectPlacement;
};