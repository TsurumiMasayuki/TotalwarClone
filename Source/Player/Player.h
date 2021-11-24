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
	// AbstractComponent ����Čp������܂���
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
	//���}�b�v
	ValueMap* m_pValueMap;
	//�ΐ푊��
	IPlayer* m_pOpponentPlayer;
	//���j�b�g���X�g
	UnitContainer m_Units;
	//�`�[���ԍ�
	int m_TeamID;

	//���j�b�g�z�u�p�ʉ݂̎c��
	int m_CurrentEnergy;

	//�c��G�l���M�[�\���p�e�L�X�g
	D2DTextRenderer* m_pEnergyText;
};