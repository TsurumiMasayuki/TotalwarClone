#include "UnitSelector.h"
#include "Actor\Base\GameObject.h"
#include "Component\Box2D\CircleColliderB2.h"
#include "Component\Cursor.h"
#include "Device\GameDevice.h"
#include "Unit\Unit.h"
#include "Unit\UnitObject.h"
#include "Math\MathUtility.h"

void UnitSelector::onStart()
{
	m_InputInterval.setUseUnscaledTime(true);
	m_InputInterval.setMaxTime(0.25f);
}

void UnitSelector::onUpdate()
{
	//3D空間上でカーソルが指している場所を取得
	const Vec3& cursorPoint = m_pCursor->getCursorPoint();
	getTransform().setLocalPosition(cursorPoint);

	//入力インターバル更新
	m_InputInterval.update();
	//入力インターバル中ならreturn
	if (!m_InputInterval.isTime()) return;

	//中クリックしたら選択(ターゲットがいないなら選択解除)
	if (GameDevice::getInput().isMouseButtonDown(2) &&
		m_pTargetUnit != nullptr)
	{
		selectUnit(m_pTargetUnit);
	}

	//左クリックしたら移動または攻撃
	if (GameDevice::getInput().isMouseButtonDown(0) &&
		m_pSelectedUnit != nullptr)
	{
		if (m_pAttackTargetUnit != nullptr)
		{
			m_pSelectedUnit->setTarget(m_pAttackTargetUnit);
		}

		//ユニット配置の記録開始
		m_UnitPlacePosBegin = cursorPoint;
	}

	//攻撃中でないなら移動処理終了
	if (GameDevice::getInput().isMouseButtonUp(0) &&
		m_pSelectedUnit != nullptr &&
		m_pAttackTargetUnit == nullptr)
	{
		//ユニット配置の記録終了
		Vec3 unitPlacePosEnd = cursorPoint;
		Vec3 diff = unitPlacePosEnd - m_UnitPlacePosBegin;
		Vec3 placePos = m_UnitPlacePosBegin + diff.normalized() * diff.length() * 0.5f;
		m_pSelectedUnit->setDestination(placePos,
			MathUtility::toDegree(std::atan2f(diff.y, diff.x)),
			diff.length() / m_pSelectedUnit->getSpacePerObject()
		);
	}
}

void UnitSelector::onTriggerStay(GameObject* pHit)
{
	auto pUnitObject = pHit->getComponent<UnitObject>();
	if (pUnitObject == nullptr) return;
	if (pUnitObject->getTeamID() != m_TeamID)
	{
		//違うチームなら攻撃対象候補にする
		m_pAttackTargetUnit = pUnitObject->getUnit();
		return;
	}
	m_pTargetUnit = pUnitObject->getUnit();
}

void UnitSelector::onTriggerExit(GameObject* pHit)
{
	auto pUnitObject = pHit->getComponent<UnitObject>();
	if (pUnitObject == nullptr) return;
	if (pUnitObject->getTeamID() == m_TeamID) return;

	//ターゲット解除
	if (pUnitObject->getUnit() == m_pTargetUnit)
	{
		m_pTargetUnit = nullptr;
	}

	//攻撃対象解除
	if (pUnitObject->getUnit() == m_pAttackTargetUnit)
	{
		m_pAttackTargetUnit = nullptr;
	}
}

void UnitSelector::init(Cursor* pCursor, int teamID)
{
	m_pCursor = pCursor;
	m_TeamID = teamID;

	//ユニット検出用コライダー追加
	auto pCollider = getUser().addComponent<CircleColliderB2>();
	pCollider->setRadius(1.0f);
	pCollider->setTrigger(true);
	pCollider->setBodyType(b2_staticBody);
}

void UnitSelector::selectUnit(Unit* pUnit)
{
	m_pSelectedUnit = pUnit;
	m_InputInterval.reset();
}
