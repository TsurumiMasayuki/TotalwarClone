#pragma once
#include "Utility\Timer.h"

//�G�t�F�N�g�N���X(��)
class IEffect;

class UnitObject;

//�U���N���X
class Attack
{
public:
	Attack(float damage,
		float attackInterval,
		float attackRange,
		float trackSpeed,
		bool isTrackTarget,
		int attackCount,
		bool isInfiniteAttack,
		UnitObject* pOwner);
	virtual ~Attack();

	void update();

	void setActive(bool isActive);
	bool getActive() const;

	void setTarget(UnitObject* pTarget);
	UnitObject* getTarget() const;

	float getAttackRange() const;

private:
	//�˒������̔���
	bool isInRange() const;
	//�^�[�Q�b�g���U��
	void attackTarget();
	//�^�[�Q�b�g�̒ǐ�
	void trackTarget();

private:
	//�U����
	const float m_Damage;
	//�U���Ԋu
	const float m_AttackInterval;
	//�˒�
	const float m_AttackRange;
	//�Ǐ]���x(��]���x)
	const float m_TrackSpeed;
	//�Ǐ]���邩�ǂ���(�Œ�C�Ɏg��)
	const bool m_IsTrackTarget;
	//�U���\��
	const int m_MaxAttackCount;
	//�U���\�񐔂��������ǂ���
	const bool m_IsInfiniteAttack;
	//�U���G�t�F�N�g(��)
	const IEffect* m_pAttackEffect;

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