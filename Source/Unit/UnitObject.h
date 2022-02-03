#pragma once
#include <vector>
#include <unordered_set>
#include "Component\Base\AbstractComponent.h"
#include "Utility\Timer.h"

class Unit;
class Attack;
class CircleColliderB2;
class ValueMap;
class TestEffect_Beam;
class CubeTrailEffect;
class EffectRenderHelper;

class UnitObject
	: public AbstractComponent
{
public:
	virtual void onStart() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;

	void init(Unit* pUnit, ValueMap* pValueMap, EffectRenderHelper* pEffectRenderHelper);

	//目的地を設定
	void setDestination(const Vec3& destination, bool isMoveCommand = true);

	//コライダーをリセット
	void resetCollider();

	void onCollisionEnter(UnitObject* pUnitObject);
	void onCollisionStay(UnitObject* pUnitObject);
	void onCollisionExit(UnitObject* pUnitObject);

	void onTriggerEnter(UnitObject* pUnitObject);
	void onTriggerStay(UnitObject* pUnitObject);
	void onTriggerExit(UnitObject* pUnitObject);

	int getTeamID() const;
	bool isDead() const;

	void takeDamage(float damage);
	float getHealth() const;
	float getShield() const;

	Unit* getUnit();

private:
	void updateShield();
	void updateAttack();
	void updateTrailEffect();

	void move();
	void rotate();

private:
	//目的地
	Vec3 m_Destination;

	//自身の方向
	Vec3 m_Direction;

	//各種判定用コライダー
	CircleColliderB2* m_pCollider;
	CircleColliderB2* m_pTrigger;

	//自分の所属しているユニット
	Unit* m_pUnit;

	//攻撃対象の候補
	std::unordered_set<UnitObject*> m_TargetCandidates;

	ValueMap* m_pValueMap;

	float m_Health;
	float m_Shield;

	bool m_IsDead;

	//シールド回復開始までのタイマー
	Timer m_ShieldRegenTimer;

	//射程距離が最も長い攻撃
	Attack* m_pLongestMainAttack;

	//攻撃クラス(メイン)
	std::vector<Attack*> m_MainAttacks;

	//攻撃クラス(サブ)
	std::vector<Attack*> m_SubAttacks;

	//エフェクト
	CubeTrailEffect* m_pCubeTrailEffect;
};