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
	//���}�b�v
	ValueMap* m_pValueMap;
	//�ΐ푊��
	IPlayer* m_pOpponentPlayer;
	//���j�b�g���X�g�B��Ń��j�b�g��p�R���e�i�ɍ����ւ���
	std::vector<Unit*> m_Units;

	int m_TeamNum;
};