#pragma once
#include "Component\Base\AbstractComponent.h"

class IPlayer;
class UISlider;

class UIBattleSlider
	: public AbstractComponent
{
public:
	virtual void onStart() override;
	virtual void onUpdate() override;

	void init(IPlayer* pPlayer, IPlayer* pEnemy);

private:
	IPlayer* m_pPlayer;
	IPlayer* m_pEnemy;

	UISlider* m_pPlayerSlider;
	UISlider* m_pEnemySlider;
};