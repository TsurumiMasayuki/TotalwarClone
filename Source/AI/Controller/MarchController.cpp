#include "MarchController.h"

#include <DirectXMath.h>
#include "Math\MathUtility.h"
#include "Unit\UnitContainer.h"

MarchController::MarchController(Unit* pUnit, IPlayer* pPlayer, const Vec3& relativePos, int unitWidth)
	: m_pUnit(pUnit),
	m_pPlayer(pPlayer),
	m_pSlowestUnit(nullptr),
	m_RelativePos(relativePos),
	m_UnitWidth(unitWidth)
{
	const auto& sortedUnits = m_pPlayer->getUnitContainer()->getSortedUnits(UnitStatsValues::Speed);
	//��ԑ��̒x�����j�b�g������
	m_pSlowestUnit = sortedUnits.at(0);
}

void MarchController::start()
{
}

void MarchController::update()
{
	float angle = m_pSlowestUnit->getAngle();
	float radian = MathUtility::toRadian(angle);

	//��]�s����v�Z
	DirectX::XMMATRIX rotate = DirectX::XMMatrixRotationRollPitchYaw(0.0f, angle, 0.0f);

	//��Ԓx�����j�b�g�ɍ��킹�Ĉړ�
	const Vec3& basePos = m_pSlowestUnit->getTransform().getLocalPosition();
	Vec3 rotatePos = m_RelativePos.multMatrix(rotate);
	m_pUnit->setDestination(basePos + rotatePos, angle, m_UnitWidth);
}
