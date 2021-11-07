#include "MarchController.h"

#include <DirectXMath.h>
#include "Component\Utility\Transform.h"
#include "Math\MathUtility.h"
#include "Unit\UnitContainer.h"

MarchController::MarchController(Unit* pUnit, IPlayer* pPlayer, const Vec3& relativePos, int unitWidth)
	: m_pUnit(pUnit),
	m_pPlayer(pPlayer),
	m_pBaseUnit(nullptr),
	m_RelativePos(relativePos),
	m_UnitWidth(unitWidth)
{
	//�x�[�X�ɂȂ郆�j�b�g������
	m_pBaseUnit = m_pPlayer->getUnitContainer()->getCenterUnit();
}

void MarchController::start()
{
}

void MarchController::update()
{
	//�퓬���Ȃ疳��
	if (m_pUnit->isInCombat())
		return;

	float angle = 0.0f;
	float radian = MathUtility::toRadian(angle);

	//��]�s����v�Z
	DirectX::XMMATRIX rotate = DirectX::XMMatrixRotationRollPitchYaw(0.0f, radian, 0.0f);

	//��Ԓx�����j�b�g�ɍ��킹�Ĉړ�
	const Vec3& basePos = m_pBaseUnit->getTransform().getLocalPosition();
	Vec3 rotatePos = m_RelativePos.multMatrix(rotate);
	m_pUnit->setDestination(basePos + rotatePos, angle, m_UnitWidth, false);
}
