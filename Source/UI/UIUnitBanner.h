#pragma once
#include "Component\Base\AbstractComponent.h"

class Unit;
class UnitSelector;

class UIUnitBanner
	: public AbstractComponent 
{
public:
	virtual void onStart() override;
	virtual void onUpdate() override;

	void init(Unit* pSourceUnit);

private:
	//�A�C�R����Transform
	Transform* m_pIconTransform;
	//�_��Transform
	Transform* m_pStickTransform;

	Unit* m_pSourceUnit;};