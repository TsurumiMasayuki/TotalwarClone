#pragma once

#include "AI\ValueMap.h"
#include "Component\Base\AbstractComponent.h"
#include "Player\IPlayer.h"
#include "Unit\UnitContainer.h"

class AIPlayer
	: public AbstractComponent, public IPlayer
{
public:
	AIPlayer();
	~AIPlayer();

	virtual void onStart() override;
	virtual void onUpdate() override;

	virtual void addUnit(Unit* pUnit) override;
	virtual UnitContainer* getUnitContainer() override;

	virtual void init(int teamNum, IPlayer* pOpponentPlayer, ValueMap* pValueMap) override;

private:
	//情報マップ
	ValueMap* m_pValueMap;
	//対戦相手
	IPlayer* m_pOpponentPlayer;
	//ユニットリスト
	UnitContainer m_Units;

	int m_TeamNum;
};