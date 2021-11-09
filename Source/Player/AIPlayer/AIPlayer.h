#pragma once
#include <unordered_map>
#include "AI\ValueMap.h"
#include "Component\Base\AbstractComponent.h"
#include "Player\IPlayer.h"
#include "Unit\UnitContainer.h"

class AbstractController;

//�e�X�g�pAI�v���C���[
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
	//���}�b�v
	ValueMap* m_pValueMap;
	//�ΐ푊��
	IPlayer* m_pOpponentPlayer;
	//���j�b�g���X�g
	UnitContainer m_Units;
	//�`�[���ԍ�
	int m_TeamID;
	//���j�b�g�P�ɂ��P�̃R���g���[���[�����蓖�Ă�̂ŁAmap�ŊǗ�
	std::unordered_map<Unit*, AbstractController*> m_Controllers;
};