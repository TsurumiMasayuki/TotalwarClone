#include "Unit.h"

#include <algorithm>

#include "Actor\Base\GameObject.h"
#include "Component\Physics\SphereColliderBt.h"
#include "Math\MathUtility.h"

#include "AI\ValueMap.h"
#include "Component\Box2D\CircleColliderB2.h"
#include "Unit\UnitObject.h"
#include "Unit\UnitRenderHelper.h"

#include "Player\IPlayer.h"

//TODO:回転を二次元座標用に変える

void Unit::onStart()
{
}

void Unit::onUpdate()
{
	m_StateLockTimer.update();

	//生きているオブジェクトがいないならreturn
	if (getObjectCount() == 0)
		return;

	if (m_pTargetUnit != nullptr)
	{
		if (m_pTargetUnit->getObjectCount() == 0)
			m_pTargetUnit = nullptr;
	}

	//インスタンシング用情報を送る
	std::vector<DirectX::XMMATRIX> objMatrices;
	for (auto pUnitObject : m_UnitObjects)
	{
		if (pUnitObject->getState() != UnitObject::State::Dead)
			objMatrices.emplace_back(pUnitObject->getTransform().getWorldMatrix());
	}
	m_pRenderHelper->appendInstanceInfo(objMatrices);

	updateCenterPosition();

	//攻撃指示があるならターゲットに向けて移動
	if (m_pTargetUnit != nullptr)
	{
		const Vec3& pos = getTransform().getLocalPosition();
		const Vec3& destination = m_pTargetUnit->getTransform().getLocalPosition();
		Vec3 diff = destination - pos;
		diff.y = 0.0f;
		//ターゲットとの角度を求める
		float radian = Vec3::dot(diff, Vec3(0.0f, 0.0f, 1.0f));

		for (auto pUnitObject : m_UnitObjects)
		{
			pUnitObject->setDestination(destination, false);
		}
	}
}

void Unit::onDestroy()
{
	for (auto pUnitObject : m_UnitObjects)
	{
		pUnitObject->getUser().destroy();
	}

	m_pOwnerPlayer->removeUnit(this);
}

void Unit::onEnable()
{
	for (auto pUnitObject : m_UnitObjects)
	{
		pUnitObject->getUser().setActive(true);
	}
}

void Unit::onDisable()
{
	for (auto pUnitObject : m_UnitObjects)
	{
		pUnitObject->getUser().setActive(false);
	}

	m_pOwnerPlayer->removeUnit(this);
}

void Unit::init(IPlayer* pPlayer, const UnitStats* pUnitStats, ValueMap* pValueMap, UnitRenderHelper* pRenderHelper)
{
	int objectCount = pUnitStats->m_ObjectCount;
	m_TeamID = pPlayer->getTeamID();
	m_pUnitStats = pUnitStats;
	m_pValueMap = pValueMap;
	m_pRenderHelper = pRenderHelper;
	m_pOwnerPlayer = pPlayer;

	//ステートロックタイマー初期化
	m_StateLockTimer.setMaxTime(1.0f);

	int xSize = objectCount;
	int zSize = objectCount / xSize;

	Vec3 basePos(-pUnitStats->m_SpacePerObject * (float)xSize * 0.5f, -10.0f, pUnitStats->m_SpacePerObject * (float)zSize * 0.5f);

	m_ObjectPlacement.setSpacePerObject(pUnitStats->m_SpacePerObject);
	m_ObjectPlacement.setBasePos(basePos);
	m_ObjectPlacement.setWidth(xSize);
	m_ObjectPlacement.setAngle(0.0f);

	for (int z = 0; z < objectCount; z++)
	{
		//ゲームオブジェクトを追加
		m_GameObjects.emplace_back();
		auto pObj = m_GameObjects.back();
		pObj = new GameObject(getUser().getGameMediator());
		auto& transform = pObj->getTransform();
		transform.setLocalScale(m_pUnitStats->m_ObjectSize);

		//コンポーネントを追加
		m_UnitObjects.emplace_back();
		auto& pUnitObject = m_UnitObjects.back();
		pUnitObject = pObj->addComponent<UnitObject>();
		pUnitObject->init(this, m_pValueMap);

		//オブジェクト配置に登録
		m_ObjectPlacement.addObject(&transform);
	}
}

void Unit::setPosition(const Vec3& position, float angle, int unitWidth)
{
	//ユニットオブジェクトの座標設定
	m_ObjectPlacement.setBasePos(position);
	m_ObjectPlacement.setAngle(angle);
	m_ObjectPlacement.setWidth(unitWidth);
	//座標計算して適用
	m_ObjectPlacement.applyObjectPositions();

	//コライダーをリセット
	for (auto pUnitObject : m_UnitObjects)
	{
		pUnitObject->setDestination(pUnitObject->getTransform().getLocalPosition());
		pUnitObject->resetCollider();
	}

	m_Width = unitWidth;

	//中心座標を更新
	updateCenterPosition();
}

void Unit::setDestination(const Vec3& destination, float angle, int unitWidth, bool isMoveCommand)
{
	//移動指示が入ったので攻撃目標はクリアする
	m_pTargetUnit = nullptr;

	m_ObjectPlacement.setBasePos(destination);
	m_ObjectPlacement.setWidth(unitWidth);
	m_ObjectPlacement.setAngle(angle);

	std::vector<Vec3> newPositions;
	m_ObjectPlacement.calculateObjectPositions(newPositions);

	for (int i = 0; i < (int)newPositions.size(); i++)
	{
		m_UnitObjects.at(i)->setDestination(newPositions.at(i), isMoveCommand);
	}

	//ステートロック開始
	m_StateLockTimer.reset();
}

float Unit::getSpacePerObject() const
{
	return m_ObjectPlacement.getSpacePerObject();
}

float Unit::getAngle() const
{
	return m_ObjectPlacement.getAngle();
}

int Unit::getWidth() const
{
	return m_Width;
}

void Unit::setTarget(Unit* pTarget)
{
	//同じものが既に設定済みなら設定しない
	if (m_pTargetUnit == pTarget) return;

	//ステートロック開始
	m_StateLockTimer.reset();

	m_pTargetUnit = pTarget;

	//目的地設定
	const Vec3& destination = m_pTargetUnit->getTransform().getLocalPosition();
	for (auto pUnitObject : m_UnitObjects)
	{
		pUnitObject->setDestination(destination);
	}
}

Unit* Unit::getTarget()
{
	return m_pTargetUnit;
}

float Unit::getHealth() const
{
	//オブジェクトからHPを取得して合計する
	float healthSum = 0.0f;
	for (UnitObject* pUnitObject : m_UnitObjects)
	{
		healthSum += pUnitObject->getHealth();
	}

	return healthSum;
}

float Unit::getShield() const
{
	//オブジェクトからシールドのHPを取得して合計する
	float shieldSum = 0.0f;
	for (UnitObject* pUnitObject : m_UnitObjects)
	{
		shieldSum += pUnitObject->getShield();
	}

	return shieldSum;
}

int Unit::getObjectCount() const
{
	int count = 0;
	for (UnitObject* pUnitObject : m_UnitObjects)
	{
		if (pUnitObject->getHealth() > 0.0f)
			count++;
	}

	return count;
}

const UnitStats* Unit::getUnitStats() const
{
	return m_pUnitStats;
}

int Unit::getTeamID() const
{
	return m_TeamID;
}

bool Unit::isStateLocked()
{
	return m_StateLockTimer.isTime();
}

void Unit::onEnterCombat(Unit* pEnemyUnit)
{
	if (m_pTargetUnit != nullptr) return;

	setTarget(pEnemyUnit);
}

void Unit::forceEscapeCombat()
{
	for (auto pUnitObject : m_UnitObjects)
	{
		pUnitObject->forceEscapeCombat();
	}

	m_StateLockTimer.reset();
}

void Unit::setAllowCombat(bool value)
{
	m_IsAllowCombat = value;
}

bool Unit::isAllowCombat() const
{
	return m_IsAllowCombat;
}

bool Unit::isInCombat() const
{
	return m_pTargetUnit != nullptr;
}

void Unit::updateCenterPosition()
{
	int aliveCount = 0;
	//生きているオブジェクトをカウント
	for (auto pUnitObject : m_UnitObjects)
	{
		if (pUnitObject->getState() != UnitObject::State::Dead)
			aliveCount++;
	}

	//ユニットの中心座標を更新
	Vec3 sum;
	for (auto pUnitObject : m_UnitObjects)
	{
		sum += pUnitObject->getTransform().getLocalPosition();
	}
	getTransform().setLocalPosition(sum / (float)aliveCount);
}
