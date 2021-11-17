#pragma once
#include "Utility\Timer.h"

class UnitObject;
struct AttackStats;
class TestEffect_Beam;

//攻撃クラス
class Attack
{
public:
	Attack(UnitObject* pOwner, const AttackStats* pAttackStats, TestEffect_Beam* pEffect);
	virtual ~Attack();

	void update();

	void setActive(bool isActive);
	bool getActive() const;

	void setTarget(UnitObject* pTarget);
	UnitObject* getTarget() const;

	float getAttackRange() const;

private:
	//ターゲットを攻撃
	void attackTarget();
	//ターゲットの追跡
	void trackTarget();

private:
	//攻撃のステータスへの参照
	const AttackStats* m_pAttackStats;

	//有効かどうか
	bool m_IsActive;

	//タイマークラス
	Timer m_AttackTimer;

	//自身が乗っているオブジェクト
	UnitObject* m_pOwner;

	//ターゲット
	UnitObject* m_pTarget;

	TestEffect_Beam* m_pEffect;

	//現在の角度
	float m_CurrentAngle;
};