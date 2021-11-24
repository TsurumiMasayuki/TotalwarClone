#pragma once
#include "AI\ValueMap.h"
#include "Component\Base\AbstractComponent.h"
#include "Player\IPlayer.h"
#include "Unit\UnitContainer.h"

class D2DTextRenderer;

class Player
	: public AbstractComponent, public IPlayer
{
public:
	// AbstractComponent を介して継承されました
	virtual void onStart() override;
	virtual void onUpdate() override;

	virtual int getTeamID() override;
	virtual void addUnit(Unit* pUnit) override;
	virtual void removeUnit(Unit* pUnit) override;
	virtual UnitContainer* getUnitContainer() override;
	virtual void init(int teamNum, IPlayer* pOpponentPlayer, ValueMap* pValueMap) override;
	virtual bool isDefeat() override;

	void setEnergy(int energy);
	int getEnergy() const;

private:
	//情報マップ
	ValueMap* m_pValueMap;
	//対戦相手
	IPlayer* m_pOpponentPlayer;
	//ユニットリスト
	UnitContainer m_Units;
	//チーム番号
	int m_TeamID;

	//ユニット配置用通貨の残り
	int m_CurrentEnergy;

	//残りエネルギー表示用テキスト
	D2DTextRenderer* m_pEnergyText;
};