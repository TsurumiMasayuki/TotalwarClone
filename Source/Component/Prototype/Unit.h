#pragma once
#include <vector>
#include <unordered_map>
#include "Component\Base\AbstractComponent.h"
#include "Component\Graphics\InstancedRenderer.h"
#include "Utility\Timer.h"

struct UnitInstanceInfo
{
	DirectX::XMFLOAT4X4 instanceMat;
	DirectX::XMFLOAT4 instanceColor;
};

class ValueMap;
class UnitObject;
struct UnitStats;

class Unit
	: public AbstractComponent
{
public:
	virtual void onStart() override;
	virtual void onUpdate() override;

	void init(int unitCount, float spacePerObject, int teamID, UnitStats* pUnitStats, ValueMap* pValueMap);
	void setPosition(const Vec3& position, float angle);
	void setDestination(const Vec3& destination, float angle);

	void setTarget(Unit* pTarget);
	Unit* getTarget();

	const UnitStats* getUnitStats() const;

	int getTeamID() const;

	bool isStateLocked();

	void onEnterCombat(Unit* pEnemyUnit);

	void calculateObjectPositions(std::vector<Vec3>& results, const Vec3& destination, float radian);

private:
	std::vector<GameObject*> m_GameObjects;
	std::vector<UnitObject*> m_UnitObjects;

	Unit* m_pTargetUnit;

	int m_UnitCount;
	float m_SpacePerObject;
	int m_TeamID;

	Timer m_StateLockTimer;
	InstancedRenderer<UnitInstanceInfo>* m_pInstancedRenderer;

	UnitStats* m_pUnitStats;
	ValueMap* m_pValueMap;
};