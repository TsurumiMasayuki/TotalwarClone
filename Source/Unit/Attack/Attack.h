#pragma once
#include "Utility\Timer.h"

//�G�t�F�N�g�N���X(��)
class IEffect;
class AttackStats;
class UnitObject;

//�U���N���X
class Attack
{
public:
	Attack(UnitObject* pOwner, const AttackStats* pAttackStats);
	virtual ~Attack();

	void update();

	void setActive(bool isActive);
	bool getActive() const;

	void setTarget(UnitObject* pTarget);
	UnitObject* getTarget() const;

	float getAttackRange() const;

private:
	//�^�[�Q�b�g���U��
	void attackTarget();
	//�^�[�Q�b�g�̒ǐ�
	void trackTarget();

private:
	//�U���̃X�e�[�^�X�ւ̎Q��
	const AttackStats* m_pAttackStats;

	//�L�����ǂ���
	bool m_IsActive;

	//�^�C�}�[�N���X
	Timer m_AttackTimer;

	//���g������Ă���I�u�W�F�N�g
	const UnitObject* m_pOwner;

	//�^�[�Q�b�g
	UnitObject* m_pTarget;

	//���݂̊p�x
	float m_CurrentAngle;
};