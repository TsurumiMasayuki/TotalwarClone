#pragma once

#include "AI\ValueMap.h"
#include "Component\Base\AbstractComponent.h"
#include "Player\IPlayer.h"

class AIPlayer
	: public AbstractComponent, public IPlayer
{
public:
	AIPlayer();
	~AIPlayer();

	virtual void onStart() override;
	virtual void onUpdate() override;

	virtual void addUnit(Unit* pUnit) override;
	virtual const std::vector<Unit*>& getUnits() override;

	virtual void init(int teamNum, IPlayer* pOpponentPlayer) override;

private:
	//情報マップ
	ValueMap* m_pValueMap;
	//対戦相手
	IPlayer* m_pOpponentPlayer;
	//ユニットリスト。後でユニット専用コンテナに差し替える
	std::vector<Unit*> m_Units;

	int m_TeamNum;
};