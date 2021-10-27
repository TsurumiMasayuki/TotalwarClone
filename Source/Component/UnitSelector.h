#pragma once
#include "Component\Base\AbstractComponent.h"
#include "Utility\Timer.h"

class Cursor;
class Unit;
class CircleColliderB2;

class UnitSelector
	: public AbstractComponent
{
public:
	virtual void onStart() override;
	virtual void onUpdate() override;

	virtual void onTriggerStay(GameObject* pHit) override;
	virtual void onTriggerExit(GameObject* pHit) override;

	void init(Cursor* pCursor, int teamID);
	void selectUnit(Unit* pUnit);

private:
	Cursor* m_pCursor;
	int m_TeamID;
	Unit* m_pTargetUnit;
	Unit* m_pSelectedUnit;
	Unit* m_pAttackTargetUnit;
	CircleColliderB2* m_pCollider;

	Vec3 m_UnitPlacePosBegin;

	//入力のインターバル
	Timer m_InputInterval;
};