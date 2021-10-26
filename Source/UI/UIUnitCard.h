#pragma once
#include "Component\Base\AbstractComponent.h"

class Unit;

class UIUnitCard
	: public AbstractComponent
{
public:
	virtual void onStart() override;
	virtual void onUpdate() override;

	void init(Unit* pUnit);

private:
	Unit* m_pUnit;
};