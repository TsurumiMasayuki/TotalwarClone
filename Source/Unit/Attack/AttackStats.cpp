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

AttackStats::AttackStats(nlohmann::json& json)
	: m_Damage((float)json["Damage"]),
	m_AttackInterval((float)json["AttackInterval"]),
	m_AttackRange((float)json["AttackRange"]),
	m_TrackSpeed((float)json["TrackSpeed"]),
	m_IsTrackTarget((bool)json["IsTrackTarget"]),
	m_MaxAttackCount((int)json["MaxAttackCount"]),
	m_IsInfiniteAttack((bool)json["IsInfiniteAttack"]),
	m_pAttackEffect(nullptr)
{
}
