#pragma once
#include "Utility\Timer.h"

//エフェクトクラス(仮)
class IEffect;

class UnitObject;

//攻撃クラス
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
	//射程距離の判定
	bool isInRange() const;
	//ターゲットを攻撃
	void attackTarget();
	//ターゲットの追跡
	void trackTarget();

private:
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

	//有効かどうか
	bool m_IsActive;

	//タイマークラス
	Timer m_AttackTimer;

	//自身が乗っているオブジェクト
	const UnitObject* m_pOwner;

	//ターゲット
	UnitObject* m_pTarget;

	//現在の角度
	float m_CurrentAngle;
};