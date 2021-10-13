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
}

void UnitSelector::onUpdate()
{
	//3D空間上でカーソルが指している場所を取得
	const Vec3& cursorPoint = m_pCursor->getCursorPoint();
	getTransform().setLocalPosition(cursorPoint);

	//左クリックしたら選択(ターゲットがいないなら選択解除)
	if (GameDevice::getInput().isMouseButtonDown(2) &&
		m_pTargetUnit != nullptr)
	{
		m_pSelectedUnit = m_pTargetUnit;
	}

	//右クリックしたら移動
	if (GameDevice::getInput().isMouseButtonDown(0) &&
		m_pSelectedUnit != nullptr)
	{
		//ユニット配置の記録開始
		m_UnitPlacePosBegin = cursorPoint;
	}

	if (GameDevice::getInput().isMouseButtonUp(0) &&
		m_pSelectedUnit != nullptr)
	{
		//ユニット配置の記録終了
		Vec3 unitPlacePosEnd = cursorPoint;
		Vec3 diff = unitPlacePosEnd - m_UnitPlacePosBegin;
		Vec3 placePos = m_UnitPlacePosBegin + diff.normalized() * diff.length() * 0.5f;
		m_pSelectedUnit->setDestination(placePos,
			MathUtility::toDegree(std::atan2f(diff.y, diff.x)) + 90.0f,
			diff.length() / m_pSelectedUnit->getSpacePerObject()
		);
	}
}

void UnitSelector::onTriggerStay(GameObject* pHit)
{
	auto pUnitObject = pHit->getComponent<UnitObject>();
	if (pUnitObject == nullptr) return;
	if (pUnitObject->getTeamID() != m_TeamID) return;

	m_pTargetUnit = pUnitObject->getUnit();
}

void UnitSelector::onTriggerExit(GameObject* pHit)
{
	auto pUnitObject = pHit->getComponent<UnitObject>();
	if (pUnitObject == nullptr) return;
	if (pUnitObject->getTeamID() == m_TeamID) return;
	if (pUnitObject->getUnit() != m_pTargetUnit) return;

	m_pTargetUnit = nullptr;
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
