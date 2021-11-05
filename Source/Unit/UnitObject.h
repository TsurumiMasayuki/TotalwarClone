#pragma once
#include <vector>
#include "Component\Base\AbstractComponent.h"
#include "Utility\Timer.h"

class Unit;
class Attack;
class CircleColliderB2;
class ValueMap;

class UnitObject
	: public AbstractComponent
{
public:
	enum class State
	{
		StandBy,//待機(通常)
		Move,	//通常移動
		Charge,	//突撃(攻撃する位置まで移動)
		Attack, //戦闘
		Dead,	//死亡
	};

public:
	virtual void onStart() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;

	void init(Unit* pUnit, ValueMap* pValueMap);

	//座標を変更
	void setPosition(const Vec3& position);
	//目的地を設定
	void setDestination(const Vec3& destination, bool isMoveCommand = true);
	void move();
	void rotate();

	void onCollisionEnter(UnitObject* pUnitObject);
	void onCollisionStay(UnitObject* pUnitObject);
	void onCollisionExit(UnitObject* pUnitObject);

	void onTriggerEnter(UnitObject* pUnitObject);
	void onTriggerStay(UnitObject* pUnitObject);
	void onTriggerExit(UnitObject* pUnitObject);

	const State& getState() const;

	int getTeamID() const;

	void takeDamage(float damage);
	float getHealth() const;
	float getShield() const;

	Unit* getUnit();

private:
	void stateTransition();
	void trySetTargetObject(UnitObject* pTargetObject, const State& nextState);

	void setState(const State& newState);

	void updateShield();

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

	//ターゲットオブジェクト
	UnitObject* m_pTargetObject;

	ValueMap* m_pValueMap;

	float m_Health;
	float m_Shield;

	State m_State;

	//シールド回復開始までのタイマー
	Timer m_ShieldRegenTimer;

	//攻撃クラス(メイン)
	std::vector<Attack*> m_MainAttacks;

	//攻撃クラス(サブ)
	std::vector<Attack*> m_SubAttacks;
};