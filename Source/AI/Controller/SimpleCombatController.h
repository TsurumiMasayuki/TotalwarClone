#pragma once
#include "AI\Controller\AbstractController.h"

//最も近い標的に向かうコントローラー
class SimpleCombatController
	: public AbstractController
{
public:
	SimpleCombatController(Unit* pUnit, IPlayer* pOpponentPlayer);

	virtual void start() override;
	virtual void update() override;

private:
	void searchTarget();

private:
	Unit* m_pUnit;
	IPlayer* m_pOpponentPlayer;
};