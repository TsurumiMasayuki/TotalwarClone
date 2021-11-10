#include "MarchController.h"

#include <DirectXMath.h>
#include "Component\Utility\Transform.h"
#include "Math\MathUtility.h"
#include "Unit\UnitContainer.h"

MarchController::MarchController(Unit* pUnit, Unit* pBaseUnit, const Vec3& relativePos, int unitWidth)
	: m_pUnit(pUnit),
	m_pBaseUnit(pBaseUnit),
	m_RelativePos(relativePos),
	m_UnitWidth(unitWidth)
{
}

void MarchController::start()
{
}

void MarchController::update()
{
	//í“¬’†‚È‚ç–³Œø
	if (m_pUnit->isInCombat())
		return;

	float angle = 0.0f;
	float radian = MathUtility::toRadian(angle);

	//‰ñ“]s—ñ‚ðŒvŽZ
	DirectX::XMMATRIX rotate = DirectX::XMMatrixRotationRollPitchYaw(0.0f, radian, 0.0f);

	//ˆê”Ô’x‚¢ƒ†ƒjƒbƒg‚É‡‚í‚¹‚ÄˆÚ“®
	const Vec3& basePos = m_pBaseUnit->getTransform().getLocalPosition();
	Vec3 rotatePos = m_RelativePos.multMatrix(rotate);
	m_pUnit->setDestination(basePos + rotatePos, angle, m_UnitWidth, false);
}
