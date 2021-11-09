#pragma once
#include <unordered_map>
#include "AI\ValueMap.h"
#include "Component\Base\AbstractComponent.h"
#include "Player\IPlayer.h"
#include "Unit\UnitContainer.h"

class AbstractController;

//テスト用AIプレイヤー
class AIPlayer
	: public AbstractComponent, public IPlayer
{
public:
	AIPlayer();
	~AIPlayer();

	virtual void onStart() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;

	virtual int getTeamID() override;
	virtual void addUnit(Unit* pUnit) override;
	virtual void removeUnit(Unit* pUnit) override;
	virtual UnitContainer* getUnitContainer() override;

	virtual void init(int teamNum, IPlayer* pOpponentPlayer, ValueMap* pValueMap) override;

private:
	//情報マップ
	ValueMap* m_pValueMap;
	//対戦相手
	IPlayer* m_pOpponentPlayer;
	//ユニットリスト
	UnitContainer m_Units;
	//チーム番号
	int m_TeamID;
	//ユニット１つにつき１つのコントローラーを割り当てるので、mapで管理
	std::unordered_map<Unit*, AbstractController*> m_Controllers;
};