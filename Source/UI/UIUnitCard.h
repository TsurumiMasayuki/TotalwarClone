#pragma once
#include "Component\Base\AbstractComponent.h"

struct UnitStats;

class UIUnitCard
	: public AbstractComponent
{
public:
	virtual void onStart() override;
	virtual void onUpdate() override;

	void init(const UnitStats* pUnitStats);
};