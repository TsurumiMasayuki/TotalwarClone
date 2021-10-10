#pragma once

enum class UnitStatsValues
{
	Health,
	Shield,
	Armor,
	DefencePower,
};

struct UnitStats
{
	//オブジェクトごとのHPの最大値
	float m_MaxHealthPerObject;

	//アーマー
	float m_Armor;

	//近接攻撃命中率
	int m_MeleeAttack;
	//近接防御確率
	int m_MeleeDefence;

	//近接攻撃ダメージ
	float m_MeleeDamage;

	//攻撃間隔
	float m_AttackInterval;
};