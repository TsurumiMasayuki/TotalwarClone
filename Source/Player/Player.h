#pragma once
#include "AI\ValueMap.h"
#include "Component\Base\AbstractComponent.h"
#include "Player\IPlayer.h"
#include "Unit\UnitContainer.h"

class Player
	: public AbstractComponent, public IPlayer
{
public:
	// AbstractComponent ����Čp������܂���
	virtual void onStart() override;
	virtual void onUpdate() override;

	virtual int getTeamID() override;
	virtual void addUnit(Unit* pUnit) override;
	virtual UnitContainer* getUnitContainer() override;
	virtual void init(int teamNum, IPlayer* pOpponentPlayer, ValueMap* pValueMap) override;

private:
	//���}�b�v
	ValueMap* m_pValueMap;
	//�ΐ푊��
	IPlayer* m_pOpponentPlayer;
	//���j�b�g���X�g
	UnitContainer m_Units;
	//�`�[���ԍ�
	int m_TeamID;
};