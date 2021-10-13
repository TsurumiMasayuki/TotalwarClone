#include "UnitObject.h"

#include <algorithm>

#include "Actor\Base\GameObject.h"
#include "AI\ValueMap.h"

#include "Component\Utility\Transform.h"
#include "Unit\Unit.h"
#include "Component\Box2D\CircleColliderB2.h"

#include "Device\GameDevice.h"

#include "UnitInfo\UnitStats.h"
#include "Math\MathUtility.h"

void UnitObject::onStart()
{
}

void UnitObject::onUpdate()
{
	//ステート遷移
	stateTransition();

	//自分が死んでいたら何も行わない
	if (m_State == State::Dead)
		return;

	//ヘルスの値を書き込む
	m_pValueMap->writeMap(UnitStatsValues::Health, Value(getTransform().getLocalPosition(), 25.0f, m_Health));

	m_AttackTimer.update();

	//ターゲットが死亡していたらターゲット解除
	if (m_pTargetObject != nullptr)
		if (m_pTargetObject->m_State == State::Dead)
		{
			m_pTargetObject = nullptr;
			setState(State::StandBy);
		}

	//突撃中なら目的地を更新
	if (m_State == State::Charge &&
		m_pTargetObject != nullptr)
		m_Destination = m_pTargetObject->getTransform().getLocalPosition();

	//待機中でない、かつ戦闘中でないなら移動処理
	if (m_State != State::StandBy &&
		m_State != State::Attack)
		move();
	else
		m_pCollider->setVelocity(0.0f, 0.0f);

	if (m_State == State::Attack &&
		m_pTargetObject != nullptr)
		attack();
}

void UnitObject::init(Unit* pUnit, ValueMap* pValueMap)
{
	m_pUnit = pUnit;
	setState(State::StandBy);

	const UnitStats* pUnitStats = m_pUnit->getUnitStats();
	m_Health = pUnitStats->m_MaxHealthPerObject;

	//通常コライダー
	m_pCollider = getUser().addComponent<CircleColliderB2>();
	m_pCollider->setTrigger(false);
	m_pCollider->setRadius(1.0f);

	//トリガー用コライダー
	m_pTrigger = getUser().addComponent<CircleColliderB2>();
	m_pTrigger->setTrigger(true);
	m_pTrigger->setRadius(1.5f);

	//攻撃間隔を設定
	m_AttackTimer.setMaxTime(pUnitStats->m_AttackInterval);

	m_pValueMap = pValueMap;
}

void UnitObject::setDestination(const Vec3& destination, bool moveCommand)
{
	m_Destination = destination;

	if (moveCommand)
	{
		m_pTargetObject = nullptr;
		setState(State::Move);
	}
}

void UnitObject::move()
{
	Vec3 diff = m_Destination - getTransform().getLocalPosition();
	diff.y = 0.0f;

	//目的地に近くなったら停止
	if (diff.length() < 0.25f &&
		m_State == State::Move)
	{
		m_State = State::StandBy;
		return;
	}

	//移動
	Vec3 moveDir = diff.normalized();
	m_pCollider->setVelocity(moveDir.x * 10.0f, moveDir.z * 10.0f);

	//目的地に向けて回転
	m_DesiredAngle = MathUtility::toDegree(std::atan2f(moveDir.z, moveDir.x)) + 90.0f;
	float curAngle = getTransform().getLocalAngles().y;
	getTransform().setLocalAngles(Vec3(0.0f, MathUtility::lerp(curAngle, m_DesiredAngle, GameDevice::getGameTime().getDeltaTime() * 2.0f)));
}

void UnitObject::onCollisionEnter(UnitObject* pUnitObject)
{
}

void UnitObject::onCollisionStay(UnitObject* pUnitObject)
{
	//自分が死んでいたら何も行わない
	if (m_State == State::Dead) return;

	if (pUnitObject == m_pTargetObject)
	{
		setState(State::Attack);
		//ユニットに通知
		m_pUnit->onEnterCombat(m_pTargetObject->m_pUnit);
	}

	trySetTargetObject(pUnitObject, State::Attack);
}

void UnitObject::onCollisionExit(UnitObject* pUnitObject)
{
}

void UnitObject::onTriggerEnter(UnitObject* pUnitObject)
{
}

void UnitObject::onTriggerStay(UnitObject* pUnitObject)
{
	//自分が死んでいたら何も行わない
	if (m_State == State::Dead) return;

	trySetTargetObject(pUnitObject, State::Charge);
}

void UnitObject::onTriggerExit(UnitObject* pUnitObject)
{
}

int UnitObject::getTeamID() const
{
	return m_pUnit->getTeamID();
}

void UnitObject::takeDamage(float damage)
{
	float armor = m_pUnit->getUnitStats()->m_Armor;
	//ダメージカット率計算
	float damageCut = armor / 100.0f;
	//ダメージを受ける
	m_Health -= damage * std::fmaxf((1.0f - damageCut), 0.0f);
}

float UnitObject::getHealth() const
{
	return m_Health;
}

Unit* UnitObject::getUnit()
{
	return m_pUnit;
}

const UnitObject::State& UnitObject::getState() const
{
	return m_State;
}

void UnitObject::stateTransition()
{
	//死亡処理
	if (m_Health > 0.0f) return;

	m_Health = 0.0f;
	setState(State::Dead);

	getUser().setActive(false);
}

void UnitObject::attack()
{
	if (!m_AttackTimer.isTime()) return;

	const UnitStats* pUnitStats = m_pUnit->getUnitStats();
	const UnitStats* pTargetUnitStats = m_pTargetObject->m_pUnit->getUnitStats();
	Random& random = GameDevice::getRandom();

	//防御確立と命中確立の差
	int diff = 35 + pTargetUnitStats->m_MeleeAttack - pUnitStats->m_MeleeDefence;
	//命中するか判定
	bool isHit = random.getRandom(0, 100) <= diff;

	//命中していたらダメージ
	if (isHit)
		m_pTargetObject->takeDamage(pUnitStats->m_MeleeDamage);

	m_AttackTimer.reset();
}

void UnitObject::trySetTargetObject(UnitObject* pTargetObject, const State& nextState)
{
	//ターゲットが設定済みなら実行しない
	if (m_pTargetObject != nullptr) 
		return;

	//同じチームなら処理を行わない
	if (getTeamID() == pTargetObject->getTeamID()) return;

	//ターゲット設定
	m_pTargetObject = pTargetObject;

	//ステート変更
	setState(nextState);

	if (nextState == State::Attack)
	{
		//ユニットに通知
		m_pUnit->onEnterCombat(m_pTargetObject->m_pUnit);
	}
}

void UnitObject::setState(const State& newState)
{
	if (m_State == State::Dead) return;
	if (m_State == newState) return;

	//移動以外のステート&ステートがロックされているなら実行しない。死亡は通す。
	//if (newState != State::Move && newState != State::Dead && m_pUnit->isStateLocked()) return;
	//if (newState != State::Move && newState != State::Dead) return;

	//戦闘以外のステートに移行したら前任と後任を解除する
	//if (newState != State::Attack)
	//{
	//	m_pSuccessor = nullptr;
	//	m_pPredecessor = nullptr;
	//}

	m_State = newState;
}
