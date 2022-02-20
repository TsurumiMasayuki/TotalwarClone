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
	float m_Damage;
	//UŒ‚ŠÔŠu
	float m_AttackInterval;
	//Ë’ö
	float m_AttackRange;
	//’Ç]‘¬“x(‰ñ“]‘¬“x)
	float m_TrackSpeed;
	//’Ç]‚·‚é‚©‚Ç‚¤‚©(ŒÅ’è–C‚Ég‚¤)
	bool m_IsTrackTarget;
	//UŒ‚‰Â”\‰ñ”
	int m_MaxAttackCount;
	//UŒ‚‰Â”\‰ñ”‚ª–³ŒÀ‚©‚Ç‚¤‚©
	bool m_IsInfiniteAttack;
	//UŒ‚ƒGƒtƒFƒNƒg(‰¼)
	IEffect* m_pAttackEffect;
};