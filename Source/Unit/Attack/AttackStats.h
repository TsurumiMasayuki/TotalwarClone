#pragma once

class IEffect;

class AttackStats
{
public:
	AttackStats(float damage,
		float attackInterval,
		float attackRange,
		float trackSpeed,
		bool isTrackTarget,
		int maxAttackCount,
		bool isInfiniteAttack);

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
};