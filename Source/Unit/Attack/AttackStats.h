#pragma once
#include "json.hpp"

class IEffect;

struct AttackStats
{
public:
	//’¼Ú’l‚ğ“ü‚ê‚é
	AttackStats(float damage,
		float attackInterval,
		float attackRange,
		float trackSpeed,
		bool isTrackTarget,
		int maxAttackCount,
		bool isInfiniteAttack);

	//json‚Å‰Šú‰»
	AttackStats(nlohmann::json& json);

public:
	//UŒ‚—Í
	const float m_Damage;
	//UŒ‚ŠÔŠu
	const float m_AttackInterval;
	//Ë’ö
	const float m_AttackRange;
	//’Ç]‘¬“x(‰ñ“]‘¬“x)
	const float m_TrackSpeed;
	//’Ç]‚·‚é‚©‚Ç‚¤‚©(ŒÅ’è–C‚Ég‚¤)
	const bool m_IsTrackTarget;
	//UŒ‚‰Â”\‰ñ”
	const int m_MaxAttackCount;
	//UŒ‚‰Â”\‰ñ”‚ª–³ŒÀ‚©‚Ç‚¤‚©
	const bool m_IsInfiniteAttack;
	//UŒ‚ƒGƒtƒFƒNƒg(‰¼)
	const IEffect* m_pAttackEffect;
};