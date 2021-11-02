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
	//�퓬���Ȃ疳��
	if (m_pUnit->isInCombat())
		return;

	//�p�x�v�Z�p�̃x�N�g�����v�Z
	Vec3 diff = m_pTargetUnit->getTransform().getLocalPosition() - m_pUnit->getTransform().getLocalPosition();
	//�ړI�n�ݒ�
	m_pUnit->setDestination(m_pTargetUnit->getTransform().getLocalPosition(),
		MathUtility::toDegree(std::atan2f(diff.x, diff.z)),
		m_UnitWidth,
		false
	);
}
