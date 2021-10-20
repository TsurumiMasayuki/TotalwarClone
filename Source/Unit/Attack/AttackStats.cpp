#include "AttackStats.h"

AttackStats::AttackStats(
	float damage,
	float attackInterval,
	float attackRange,
	float trackSpeed,
	bool isTrackTarget,
	int maxAttackCount,
	bool isInfiniteAttack)
	: m_Damage(damage),
	m_AttackInterval(attackInterval),
	m_AttackRange(attackRange),
	m_TrackSpeed(trackSpeed),
	m_IsTrackTarget(isTrackTarget),
	m_MaxAttackCount(maxAttackCount),
	m_IsInfiniteAttack(isInfiniteAttack),
	m_pAttackEffect(nullptr)
{
}
