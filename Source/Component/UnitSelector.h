#pragma once
#include "Component\Base\AbstractComponent.h"

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

private:
	Cursor* m_pCursor;
	int m_TeamID;
	Unit* m_pTargetUnit;
	Unit* m_pSelectedUnit;
	CircleColliderB2* m_pCollider;

	Vec3 m_UnitPlacePosBegin;
};