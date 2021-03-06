#pragma once
#include "json.hpp"

class IEffect;

struct AttackStats
{
public:
	//直接値を入れる
	AttackStats(float damage,
		float attackInterval,
		float attackRange,
		float trackSpeed,
		bool isTrackTarget,
		int maxAttackCount,
		bool isInfiniteAttack);

	//jsonで初期化
	AttackStats(nlohmann::json& json);

public:
	//攻撃力
	float m_Damage;
	//攻撃間隔
	float m_AttackInterval;
	//射程
	float m_AttackRange;
	//追従速度(回転速度)
	float m_TrackSpeed;
	//追従するかどうか(固定砲に使う)
	bool m_IsTrackTarget;
	//攻撃可能回数
	int m_MaxAttackCount;
	//攻撃可能回数が無限かどうか
	bool m_IsInfiniteAttack;
	//攻撃エフェクト(仮)
	IEffect* m_pAttackEffect;
};