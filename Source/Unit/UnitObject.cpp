#include "UnitObject.h"

#include <algorithm>

#include "Actor\Base\GameObject.h"
#include "AI\ValueMap.h"

#include "Component\Utility\Transform.h"
#include "Component\Box2D\CircleColliderB2.h"

#include "Device\GameDevice.h"

#include "Unit\Unit.h"
#include "Unit\UnitStats.h"
#include "Unit\Attack\Attack.h"
#include "Utility\JsonFileManager.h"

#include "Math\MathUtility.h"
#include "Math\Easing.h"

#include "GameState.h"

#include "Effect\TestEffect_Beam.h"

void UnitObject::onStart()
{
	m_ShieldRegenTimer.setMaxTime(1.0f);
}

void UnitObject::onUpdate()
{
	//準備フェーズ中なら何も行わない
	if (Game::g_GameState == Game::GameState::PreparePhase)
		return;

	//ステート遷移
	stateTransition();

	//自分が死んでいたら何も行わない
	if (m_State == State::Dead)
		return;

	//ヘルスの値を書き込む
	m_pValueMap->writeMap(UnitStatsValues::Health, Value(getTransform().getLocalPosition(), 25.0f, m_Health));

	updateShield();

	//ターゲットが死亡していたらターゲット解除
	if (m_pTargetObject != nullptr)
	{
		if (m_pTargetObject->m_State == State::Dead)
		{
			m_pTargetObject = nullptr;

			//攻撃を無効化
			for (auto pAttacks : m_MainAttacks)
			{
				pAttacks->setTarget(nullptr);
				pAttacks->setActive(false);
			}
		}
	}

	switch (m_State)
	{
	case UnitObject::State::StandBy:
		m_pCollider->setVelocity(0.0f, 0.0f);
		break;
	case UnitObject::State::Move:
		move();
		rotate();
		break;
	case UnitObject::State::Charge:
		if (m_pTargetObject != nullptr)
		{
			//自身の座標
			const Vec3& myPos = getTransform().getLocalPosition();
			//ターゲットの座標
			const Vec3& targetPos = m_pTargetObject->getTransform().getLocalPosition();

			//自身の座標とターゲットの座標から攻撃が当たる最も近い座標を算出して目的地にする
			float range = m_pLongestMainAttack->getAttackRange();
			Vec3 diff = targetPos - myPos;
			if (diff.sqrLength() < range * range)
			{
				m_Destination = myPos;
			}
			else
			{
				m_Destination = targetPos + diff.normalized() * range;
			}
		}
		move();
		rotate();
		break;
	case UnitObject::State::Attack:
		//メイン攻撃を更新
		for (auto pAttack : m_MainAttacks)
		{
			pAttack->update();
		}
		//サブ攻撃を更新
		for (auto pAttack : m_SubAttacks)
		{
			pAttack->update();
		}
		m_pCollider->setVelocity(0.0f, 0.0f);

		rotate();

		break;
	}
}

void UnitObject::onDestroy()
{
	//メイン攻撃のインスタンスを削除
	for (auto pAttack : m_MainAttacks)
	{
		delete pAttack;
	}

	//サブ攻撃のインスタンスを削除
	for (auto pAttack : m_SubAttacks)
	{
		delete pAttack;
	}
}

void UnitObject::init(Unit* pUnit, ValueMap* pValueMap, EffectRenderHelper* pEffectRenderHelper)
{
	m_pUnit = pUnit;
	setState(State::StandBy);

	const UnitStats* pUnitStats = m_pUnit->getUnitStats();
	m_Health = pUnitStats->m_MaxHealthPerObject;
	m_Shield = pUnitStats->m_MaxShieldPerObject;
	m_pValueMap = pValueMap;

	for (auto& attacks : m_pUnit->getUnitStats()->getMainAttacks())
	{
		auto pEffectObj = new GameObject(getUser().getGameMediator());

		//エフェクト生成
		m_AttackEffects.emplace_back(
			pEffectObj->addComponent<TestEffect_Beam>()
		);

		auto pAttackEffect = m_AttackEffects.back();
		pAttackEffect->init(pEffectRenderHelper);

		//攻撃クラス生成
		m_MainAttacks.emplace_back(
			new Attack(this, 
				&JsonFileManager<AttackStats>::getInstance().get(attacks.m_AttackName),
				pAttackEffect)
		);
	}

	//射程距離が長い順にソート
	std::sort(m_MainAttacks.begin(), m_MainAttacks.end(),
		[](Attack* a, Attack* b)
		{
			return a->getAttackRange() > b->getAttackRange();
		}
	);

	//最も射程距離が長い攻撃を登録
	m_pLongestMainAttack = m_MainAttacks.at(0);

	//通常コライダー
	m_pCollider = getUser().addComponent<CircleColliderB2>();
	m_pCollider->setTrigger(false);
	m_pCollider->setRadius(1.0f);
	m_pCollider->setBodyType(b2BodyType::b2_dynamicBody);
	m_pCollider->setGroupIndex(0);

	//トリガー用コライダー
	m_pTrigger = getUser().addComponent<CircleColliderB2>();
	m_pTrigger->setTrigger(true);
	//射程距離と同じサイズにするためにスケールで割る
	float scaleX = getTransform().getLocalScale().x;
	m_pTrigger->setRadius(m_pLongestMainAttack->getAttackRange() / scaleX);
	m_pTrigger->setGroupIndex(-1);
}

void UnitObject::setDestination(const Vec3& destination, bool isMoveCommand)
{
	//ターゲットがいて移動命令でないならreturn
	if (m_pTargetObject != nullptr &&
		!isMoveCommand &&
		m_State == State::Attack) return;

	m_Destination = destination;
	m_Destination.y = 0.0f;

	if (isMoveCommand)
	{
		m_pTargetObject = nullptr;
		setState(State::Move);
	}
}

void UnitObject::move()
{
	Vec3 diff = m_Destination - getTransform().getLocalPosition();
	diff.y = 0.0f;

	//徐々に減速
	float ratio = std::fminf(1.0f, diff.length() / m_pUnit->getUnitStats()->m_Speed);
	ratio = Easing::easeOutExpo(ratio) * m_pUnit->getUnitStats()->m_Speed;
	m_pCollider->setVelocity(m_Direction.x * ratio, m_Direction.z * ratio);
}

void UnitObject::rotate()
{
	//回転方向を求める
	Vec3 diff = m_Destination - getTransform().getLocalPosition();
	if (m_pTargetObject != nullptr)
		diff = m_pTargetObject->getTransform().getLocalPosition() - getTransform().getLocalPosition();
	diff.y = 0.0f;

	//方向を角度に変換
	float desiredAngle = MathUtility::toDegree(std::atan2f(diff.x, diff.z));

	//目的地に向けて回転
	float curAngle = getTransform().getLocalAngles().y;

	//使用する角度
	float angle = MathUtility::lerp(curAngle, desiredAngle, 1.0f);
	float radian = MathUtility::toRadian(angle);

	//移動方向を更新
	m_Direction = Vec3(std::sinf(radian), 0.0f, std::cosf(radian));

	getTransform().setLocalAngles(Vec3(0.0f,
		angle,
		0.0f)
	);
}

void UnitObject::resetCollider()
{
	m_pCollider->init();
	m_pTrigger->init();
}

void UnitObject::onCollisionEnter(UnitObject* pUnitObject)
{
}

void UnitObject::onCollisionStay(UnitObject* pUnitObject)
{
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
	m_pTargetObject = nullptr;
}

int UnitObject::getTeamID() const
{
	return m_pUnit->getTeamID();
}

void UnitObject::takeDamage(float damage)
{
	//シールドが無かったらHPでダメージを受ける。そうでなければシールドで受ける。
	if (m_Shield <= 0.0f)
	{
		m_Health -= damage;
		m_Health = std::fmaxf(0.0f, m_Health);
	}
	else
	{
		m_Shield -= damage;
		m_Shield = std::fmaxf(0.0f, m_Shield);
	}

	//シールド回復のタイマーをリセット
	m_ShieldRegenTimer.reset();

	//死亡判定
	if (m_Health <= 0.0f)
		setState(State::Dead);
}

float UnitObject::getHealth() const
{
	return m_Health;
}

float UnitObject::getShield() const
{
	return m_Shield;
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
	if (m_Health <= 0.0f)
	{
		setState(State::Dead);
		getUser().setActive(false);
		return;
	}

	const Vec3& myPos = getTransform().getLocalPosition();

	//攻撃用ステート遷移
	if (m_pTargetObject != nullptr)
	{
		const Vec3& targetPos = m_pTargetObject->getTransform().getLocalPosition();

		float sqrDistance = myPos.sqrDistance(targetPos);
		float range = m_MainAttacks.at(0)->getAttackRange();
		if (sqrDistance < range * range)
		{
			setState(State::Attack);
		}
		else
		{
			//setState(State::Charge);
		}
	}
	//移動用ステート遷移
	else
	{
		float distance = myPos.distance(m_Destination);
		if (distance > 0.15f)
		{
			setState(State::Move);
		}
		else
		{
			setState(State::StandBy);
		}
	}
}

void UnitObject::trySetTargetObject(UnitObject* pTargetObject, const State& nextState)
{
	//ターゲットが設定済みなら実行しない
	if (m_pTargetObject != nullptr) return;

	//同じチームなら実行しない
	if (getTeamID() == pTargetObject->getTeamID()) return;

	//ステートロック中なら実行しない
	if (m_pUnit->isStateLocked()) return;

	//ターゲット設定
	m_pTargetObject = pTargetObject;
	for (auto pAttack : m_MainAttacks)
	{
		//攻撃を有効化
		pAttack->setActive(true);
		pAttack->setTarget(m_pTargetObject);
	}
}

void UnitObject::setState(const State& newState)
{
	if (m_State == State::Dead) return;
	if (m_State == newState) return;

	m_State = newState;

	if (m_State == State::Attack)
	{
		//ユニットに通知
		m_pUnit->onEnterCombat(m_pTargetObject->m_pUnit);
	}
}

void UnitObject::updateShield()
{
	m_ShieldRegenTimer.update();

	//シールド回復開始
	if (m_ShieldRegenTimer.isTime())
	{
		//シールドの最大値を超えないように回復
		m_Shield = std::fminf(m_pUnit->getUnitStats()->m_MaxShieldPerObject,
			m_Shield + 10.0f * GameDevice::getGameTime().getDeltaTime());
	}
}
