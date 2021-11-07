#include "Unit.h"

#include <algorithm>

#include "Actor\Base\GameObject.h"
#include "Component\Physics\SphereColliderBt.h"
#include "Math\MathUtility.h"

#include "AI\ValueMap.h"
#include "Component\Box2D\CircleColliderB2.h"
#include "Unit\UnitObject.h"
#include "Unit\UnitRenderHelper.h"

//TODO:回転を二次元座標用に変える

void Unit::onStart()
{
}

void Unit::onUpdate()
{
	m_StateLockTimer.update();

	//インスタンシング用情報を送る
	std::vector<DirectX::XMMATRIX> objMatrices;
	for (auto pUnitObject : m_UnitObjects)
	{
		objMatrices.emplace_back(pUnitObject->getTransform().getWorldMatrix());
	}
	m_pRenderHelper->appendInstanceInfo(objMatrices);

	int aliveCount = 0;
	//生きているオブジェクトをカウント
	for (auto pUnitObject : m_UnitObjects)
	{
		if (pUnitObject->getState() != UnitObject::State::Dead)
			aliveCount++;
	}

	//生きているオブジェクトがいないならreturn
	if (aliveCount == 0) return;

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

		int i = 0;
		for (int i = 0; i < m_ObjectCount; i++)
		{
			m_UnitObjects.at(i)->setDestination(destination, false);
		}
	}
}

void Unit::init(int teamID, const UnitStats* pUnitStats, ValueMap* pValueMap, UnitRenderHelper* pRenderHelper)
{
	m_ObjectCount = pUnitStats->m_ObjectCount;
	m_SpacePerObject = pUnitStats->m_SpacePerObject;
	m_TeamID = teamID;
	m_pUnitStats = pUnitStats;
	m_pValueMap = pValueMap;
	m_pRenderHelper = pRenderHelper;

	//ステートロックタイマー初期化
	m_StateLockTimer.setMaxTime(1.0f);

	int xSize = m_ObjectCount / 1;
	int zSize = m_ObjectCount / xSize;

	Vec3 basePos(-m_SpacePerObject * (float)xSize * 0.5f, -10.0f, m_SpacePerObject * (float)zSize * 0.5f);

	for (int z = 0; z < zSize; z++)
	{
		for (int x = 0; x < xSize; x++)
		{
			//ゲームオブジェクトを追加
			m_GameObjects.emplace_back();
			auto& pObj = m_GameObjects.back();
			pObj = new GameObject(getUser().getGameMediator());

			//コンポーネントを追加
			m_UnitObjects.emplace_back();
			auto& pUnitObject = m_UnitObjects.back();
			pUnitObject = pObj->addComponent<UnitObject>();
			pUnitObject->init(this, m_pValueMap);

			//目的地を初期化
			auto& tr = pObj->getTransform();
			Vec3 position = Vec3(m_SpacePerObject * x, 0.0f, m_SpacePerObject * z) + basePos;
			tr.setLocalPosition(position);
			tr.setLocalScale(m_pUnitStats->m_ObjectSize);

			pUnitObject->setDestination(position);
		}
	}
}

void Unit::setPosition(const Vec3& position, float angle, int unitWidth)
{
	std::vector<Vec3> newPositions;
	calculateObjectPositions(newPositions, position, MathUtility::toRadian(angle), unitWidth);

	int i = 0;
	for (int i = 0; i < m_ObjectCount; i++)
	{
		m_UnitObjects.at(i)->setPosition(newPositions.at(i));
		m_UnitObjects.at(i)->setDestination(newPositions.at(i));
	}

	m_Angle = angle;

	//中心座標を更新
	updateCenterPosition();
}

void Unit::setDestination(const Vec3& destination, float angle, int unitWidth, bool isMoveCommand)
{
	//移動指示が入ったので攻撃目標はクリアする
	m_pTargetUnit = nullptr;

	std::vector<Vec3> newPositions;
	calculateObjectPositions(newPositions, destination, MathUtility::toRadian(angle), unitWidth);

	int i = 0;
	for (int i = 0; i < m_ObjectCount; i++)
	{
		m_UnitObjects.at(i)->setDestination(newPositions.at(i), isMoveCommand);
	}

	//ステートロック開始
	m_StateLockTimer.reset();

	m_Angle = angle;
}

float Unit::getSpacePerObject() const
{
	return m_SpacePerObject;
}

float Unit::getAngle() const
{
	return m_Angle;
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
	for (UnitObject* pObj : m_UnitObjects)
	{
		healthSum += pObj->getHealth();
	}

	return healthSum;
}

float Unit::getShield() const
{
	//オブジェクトからシールドのHPを取得して合計する
	float shieldSum = 0.0f;
	for (UnitObject* pObj : m_UnitObjects)
	{
		shieldSum += pObj->getShield();
	}

	return shieldSum;
}

int Unit::getObjectCount() const
{
	int count = 0;
	for (UnitObject* pObj : m_UnitObjects)
	{
		if (pObj->getHealth() > 0.0f)
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

bool Unit::isInCombat() const
{
	return m_pTargetUnit != nullptr;
}

void Unit::calculateObjectPositions(std::vector<Vec3>& results, const Vec3& destination, float radian, int unitWidth)
{
	DirectX::XMMATRIX rotateMat = DirectX::XMMatrixRotationRollPitchYaw(0.0f, radian, 0.0f);

	//列の数はユニットの数と0除算にならない値の範囲に収まるように制限
	m_UnitWidth = MathUtility::clamp(unitWidth, 1, m_ObjectCount);
	//切り上げ用に割る
	float div = (float)m_ObjectCount / (float)m_UnitWidth;
	//切り上げて行の数を求める
	div = std::ceil(div);

	int xSize = m_UnitWidth;
	int zSize = (int)div;

	//オブジェクトの数の分だけvectorのサイズ確保
	results.reserve(m_ObjectCount);

	//残りの計算回数
	int remainObjCount = m_ObjectCount;
	//オブジェクトごとの座標計算
	for (int z = zSize - 1; z >= 0; z--)
	{
		//中央揃えの為に判定
		if (remainObjCount < m_UnitWidth)
		{
			xSize = remainObjCount;
		}

		//ベースの座標を計算
		Vec3 basePos(-m_SpacePerObject * (float)(xSize - 1) * 0.5f, -10.0f, -m_SpacePerObject * (float)(zSize) * 0.5f);
		for (int x = 0; x < xSize; x++)
		{
			Vec3 newPosition = Vec3(m_SpacePerObject * x, 0.0f, m_SpacePerObject * z) + basePos;
			newPosition = newPosition.multMatrix(rotateMat) + destination;
			results.emplace_back(newPosition);
			remainObjCount--;
		}
	}
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
