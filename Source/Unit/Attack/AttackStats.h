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

	//攻撃力
	const float m_Damage;
	//攻撃間隔
	const float m_AttackInterval;
	//射程
	const float m_AttackRange;
	//追従速度(回転速度)
	const float m_TrackSpeed;
	//追従するかどうか(固定砲に使う)
	const bool m_IsTrackTarget;
	//攻撃可能回数
	const int m_MaxAttackCount;
	//攻撃可能回数が無限かどうか
	const bool m_IsInfiniteAttack;
	//攻撃エフェクト(仮)
	const IEffect* m_pAttackEffect;
};