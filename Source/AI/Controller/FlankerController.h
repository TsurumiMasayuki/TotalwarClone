#pragma once
#include "AI\Controller\AbstractController.h"

class FlankerController
	: public AbstractController
{
public:
	FlankerController(Unit* pUnit, IPlayer* pOpponentPlayer, ValueMap* pEnemyValueMap, int unitWidth);

	virtual void start() override;
	virtual void update() override;

private:
	void selectTarget();

private:
	Unit* m_pUnit;
	Unit* m_pTargetUnit;
	IPlayer* m_pOpponentPlayer;
	ValueMap* m_pEnemyValueMap;
	int m_UnitWidth;
};