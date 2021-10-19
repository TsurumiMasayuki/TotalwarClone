#pragma once
#include "Component\Base\AbstractComponent.h"

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

	//目的地を設定
	void setDestination(const Vec3& destination, bool moveCommand = true);
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

	Unit* getUnit();

private:
	void stateTransition();
	void trySetTargetObject(UnitObject* pTargetObject, const State& nextState);

	void setState(const State& newState);

private:
	//目的地
	Vec3 m_Destination;

	//各種判定用コライダー
	CircleColliderB2* m_pCollider;
	CircleColliderB2* m_pTrigger;

	//自分の所属しているユニット
	Unit* m_pUnit;

	//ターゲットオブジェクト
	UnitObject* m_pTargetObject;

	ValueMap* m_pValueMap;

	float m_Health;
	float m_DesiredAngle;

	State m_State;

	//攻撃クラス
	Attack* m_pAttack;
};