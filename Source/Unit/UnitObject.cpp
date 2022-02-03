#include "UnitObject.h"
#include <algorithm>

#include "Actor\Base\GameObject.h"

#include "Component\Utility\Transform.h"

#include "Device\GameDevice.h"

#include "AI\ValueMap.h"

#include "Component\Box2D\CircleColliderB2.h"

#include "Unit\Unit.h"
#include "Unit\UnitStats.h"
#include "Unit\Attack\Attack.h"
#include "Utility\JsonFileManager.h"

#include "Math\MathUtility.h"
#include "Math\Easing.h"

#include "GameState.h"

#include "Effect\TestEffect_Beam.h"
#include "Effect\CubeTrailEffect.h"

void UnitObject::onStart()
{
	m_ShieldRegenTimer.setMaxTime(1.0f);
}

void UnitObject::onUpdate()
{
	//準備フェーズ中なら何も行わない
	if (Game::g_GameState == Game::GameState::PreparePhase)
		return;

	//自分が死んでいたら何も行わない
	if (m_IsDead)
		return;

	//HPの値を書き込む
	m_pValueMap->writeMap(UnitStatsValues::Health, Value(getTransform().getLocalPosition(), 25.0f, m_Health));

	updateShield();

	move();
	rotate();

	updateAttack();
	updateTrailEffect();

	m_TargetCandidates.clear();
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

	const UnitStats* pUnitStats = m_pUnit->getUnitStats();
	m_Health = pUnitStats->m_MaxHealthPerObject;
	m_Shield = pUnitStats->m_MaxShieldPerObject;
	m_pValueMap = pValueMap;

	//攻撃の読み込み
	for (auto& attacks : m_pUnit->getUnitStats()->getMainAttacks())
	{
		auto pEffectObj = new GameObject(getUser().getGameMediator());

		//エフェクト生成
		auto pAttackEffect = pEffectObj->addComponent<TestEffect_Beam>();
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

	//通常コライダー
	m_pCollider = getUser().addComponent<CircleColliderB2>();
	m_pCollider->setTrigger(false);
	m_pCollider->setRadius(pUnitStats->m_ObjectSize.x);
	m_pCollider->setBodyType(b2BodyType::b2_dynamicBody);
	m_pCollider->setGroupIndex(0);

	//トリガー用コライダー
	m_pTrigger = getUser().addComponent<CircleColliderB2>();
	m_pTrigger->setTrigger(true);

	//射程距離と同じサイズにするためにスケールで割る
	//最も射程距離が長い攻撃を登録
	m_pLongestMainAttack = m_MainAttacks.at(0);
	float scaleX = pUnitStats->m_ObjectSize.x;
	m_pTrigger->setRadius(m_pLongestMainAttack->getAttackRange() / scaleX);
	m_pTrigger->setGroupIndex(-1);

	//軌跡エフェクトコンポーネント追加
	m_pCubeTrailEffect = getUser().addComponent<CubeTrailEffect>();
	m_pCubeTrailEffect->init(pEffectRenderHelper);
}

void UnitObject::setDestination(const Vec3& destination, bool isMoveCommand)
{
	m_Destination = destination;
	m_Destination.y = 0.0f;
}

void UnitObject::move()
{
	Vec3 diff = m_Destination - getTransform().getLocalPosition();
	diff.y = 0.0f;

	//徐々に減速
	float ratio = std::fminf(1.0f, diff.length() / m_pUnit->getUnitStats()->m_Speed);
	float speed = Easing::easeOutExpo(ratio) * m_pUnit->getUnitStats()->m_Speed;
	m_pCollider->setVelocity(m_Direction.x * speed, m_Direction.z * speed);
}

void UnitObject::rotate()
{
	//回転方向を求める
	Vec3 diff = m_Destination - getTransform().getLocalPosition();
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
	if (m_IsDead) return;

	//同じチームなら実行しない
	if (getTeamID() == pUnitObject->getTeamID()) return;

	//ターゲットが設定済みなら追加しない
	if (m_pUnit->getTarget() != pUnitObject->getUnit() && m_pUnit->getTarget() != nullptr) return;

	//ターゲット候補追加
	m_TargetCandidates.insert(pUnitObject);

	m_pUnit->onEnterCombat(pUnitObject->getUnit());
}

void UnitObject::onTriggerExit(UnitObject* pUnitObject)
{
}

int UnitObject::getTeamID() const
{
	return m_pUnit->getTeamID();
}

bool UnitObject::isDead() const
{
	return m_IsDead;
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
	{
		m_IsDead = true;
		m_pCollider->setVelocity(0.0f, 0.0f);
		m_pCollider->setActive(false);
		m_pTrigger->setActive(false);
		m_pCubeTrailEffect->setActive(false);
	}
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

void UnitObject::updateShield()
{
	m_ShieldRegenTimer.update();

	//シールド回復開始
	if (m_ShieldRegenTimer.isTime())
	{
		//シールドの最大値を超えないように回復
		m_Shield = std::fminf(m_pUnit->getUnitStats()->m_MaxShieldPerObject,
			m_Shield + 1000.0f * GameDevice::getGameTime().getDeltaTime());
	}
}

void UnitObject::updateAttack()
{
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

	//ターゲット設定
	const Vec3& myPosition = getTransform().getLocalPosition();

	//ターゲット候補数が攻撃の数よりも少ないなら攻撃を集中させる
	bool focusTarget = m_TargetCandidates.size() < m_MainAttacks.size();

	for (auto pAttack : m_MainAttacks)
	{
		UnitObject* pTarget = nullptr;

		for (auto pCandidate : m_TargetCandidates)
		{
			//死んでいるなら設定しない
			if (pCandidate->m_IsDead) continue;

			//距離を計算
			float sqrDistance = myPosition.sqrDistance(pCandidate->getTransform().getLocalPosition());
			//攻撃範囲内か判定
			if (pAttack->getAttackRange() * pAttack->getAttackRange() < sqrDistance) continue;

			//攻撃対象を設定
			pTarget = pCandidate;
			break;
		}

		//攻撃対象を設定
		pAttack->setTarget(pTarget);

		//設定したならリストから削除(攻撃を集中させる場合はしない)
		if (pTarget != nullptr &&
			!focusTarget)
			m_TargetCandidates.erase(pTarget);
	}
}

void UnitObject::updateTrailEffect()
{
	Vec3 diff = m_Destination - getTransform().getLocalPosition();

	//目的地に着いた場合はエフェクトを切ってreturn
	m_pCubeTrailEffect->setActive(diff.sqrLength() >= 0.5f);
}
