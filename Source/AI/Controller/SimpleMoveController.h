#pragma once
#include "AbstractController.h"

class SimpleMoveController
	: public AbstractController
{
public:
	SimpleMoveController(Unit* pUnit, Unit* pTargetUnit, int unitWidth);

	virtual void start() override;
	virtual void update() override;

private:
	Unit* m_pUnit;
	Unit* m_pTargetUnit;
	int m_UnitWidth;
};