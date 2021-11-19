#pragma once
#include <vector>
#include <unordered_map>
#include "Component\Base\AbstractComponent.h"
#include "Utility\Timer.h"

#include "Blockbench/BlockbenchModel.h"
#include "Unit\ObjectPlacement.h"

struct UnitStats;
class IPlayer;
class ValueMap;
class UnitObject;
class UnitRenderHelper;
class EffectRenderHelper;

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

	void init(IPlayer* pPlayer, const UnitStats* pUnitStats, ValueMap* pValueMap, UnitRenderHelper* pUnitRenderHelper, EffectRenderHelper* pEffectRenderHelper, bool isControlledByHuman = true);
	void setPosition(const Vec3& position, float angle, int unitWidth);
	void setDestination(const Vec3& destination, float angle, int unitWidth, bool isMoveCommand = false);

	float getSpacePerObject() const;
	float getAngle() const;
	int getWidth() const;

	void setTarget(Unit* pTarget, bool isPlayerCommand = false);
	Unit* getTarget();

	float getHealth() const;
	float getShield() const;
	int getObjectCount() const;
	const UnitStats* getUnitStats() const;

	int getTeamID() const;

	void onEnterCombat(Unit* pEnemyUnit);
	bool isInCombat() const;

	std::vector<UnitObject*>& getUnitObjects();

private:
	void updateCenterPosition();

private:
	std::vector<GameObject*> m_GameObjects;
	std::vector<UnitObject*> m_UnitObjects;

	IPlayer* m_pOwnerPlayer;

	Unit* m_pTargetUnit;

	int m_TeamID;
	//プレイヤーにコントロールされているかどうか
	bool m_IsControlledByHuman;

	ValueMap* m_pValueMap;
	const UnitStats* m_pUnitStats;
	UnitRenderHelper* m_pUnitRenderHelper;

	ObjectPlacement m_ObjectPlacement;

	float m_MainAttackRange;
};