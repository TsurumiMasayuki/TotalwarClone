#pragma once
#include "Utility\Timer.h"

class UnitObject;
struct AttackStats;
class TestEffect_Beam;

//�U���N���X
class Attack
{
public:
	Attack(UnitObject* pOwner, const AttackStats* pAttackStats, TestEffect_Beam* pEffect);
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
	UnitObject* m_pOwner;

	//�^�[�Q�b�g
	UnitObject* m_pTarget;

	TestEffect_Beam* m_pEffect;

	//���݂̊p�x
	float m_CurrentAngle;
};