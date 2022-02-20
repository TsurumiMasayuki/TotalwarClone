#pragma once
#include "json.hpp"

class IEffect;

struct AttackStats
{
public:
	//���ڒl������
	AttackStats(float damage,
		float attackInterval,
		float attackRange,
		float trackSpeed,
		bool isTrackTarget,
		int maxAttackCount,
		bool isInfiniteAttack);

	//json�ŏ�����
	AttackStats(nlohmann::json& json);

public:
	//�U����
	float m_Damage;
	//�U���Ԋu
	float m_AttackInterval;
	//�˒�
	float m_AttackRange;
	//�Ǐ]���x(��]���x)
	float m_TrackSpeed;
	//�Ǐ]���邩�ǂ���(�Œ�C�Ɏg��)
	bool m_IsTrackTarget;
	//�U���\��
	int m_MaxAttackCount;
	//�U���\�񐔂��������ǂ���
	bool m_IsInfiniteAttack;
	//�U���G�t�F�N�g(��)
	IEffect* m_pAttackEffect;
};