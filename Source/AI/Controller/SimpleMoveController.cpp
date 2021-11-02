#include "SimpleMoveController.h"
#include "Math\MathUtility.h"

SimpleMoveController::SimpleMoveController(Unit* pUnit, Unit* pTargetUnit, int unitWidth)
	: m_pUnit(pUnit),
	m_pTargetUnit(pTargetUnit),
	m_UnitWidth(unitWidth)
{
}

void SimpleMoveController::start()
{
}

void SimpleMoveController::update()
{
	//戦闘中なら無効
	if (m_pUnit->isInCombat())
		return;

	//角度計算用のベクトルを計算
	Vec3 diff = m_pTargetUnit->getTransform().getLocalPosition() - m_pUnit->getTransform().getLocalPosition();
	//目的地設定
	m_pUnit->setDestination(m_pTargetUnit->getTransform().getLocalPosition(),
		MathUtility::toDegree(std::atan2f(diff.x, diff.z)),
		m_UnitWidth,
		false
	);
}
