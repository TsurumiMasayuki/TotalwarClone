#include "FlankerController.h"

#include "Component\Utility\Transform.h"

#include "Unit\UnitContainer.h"

FlankerController::FlankerController(Unit* pUnit, IPlayer* pOpponentPlayer, ValueMap* pEnemyValueMap, int unitWidth)
	: m_pUnit(pUnit),
	m_pOpponentPlayer(pOpponentPlayer),
	m_pEnemyValueMap(pEnemyValueMap),
	m_UnitWidth(unitWidth)
{
}

void FlankerController::start()
{
}

void FlankerController::update()
{
	if (m_pTargetUnit == nullptr)
		selectTarget();

	Vec3 unitPosition = m_pUnit->getTransform().getLocalPosition();
	Vec3 offset = Vec3(50.0f, 0.0f, 0.0f);
	Vec3 destinationCandidate;

	float increment = 0.0f;
	float valueSum = 0.0f;
	float angle = 0.0f;

	//�ړI�n��������܂Ń��[�v
	while (true)
	{
		float sin = std::sinf(DirectX::XM_PI * increment);
		increment += 0.5f;

		//�I�t�Z�b�g����]
		Vec3 rotateOffset = Vec3(
			offset.x + offset.z * std::cosf(DirectX::XM_PI * 0.5f * sin),
			0.0f,
			offset.z + offset.x * std::sinf(DirectX::XM_PI * 0.5f * sin)
		);

		float value = m_pEnemyValueMap->readMap(UnitStatsValues::Health, unitPosition + rotateOffset);
		valueSum += value;

		//�ړI�n��������
		if (value < 100.0f)
		{
			destinationCandidate = unitPosition + rotateOffset;
			angle = 90.0f + 180.0f * sin;
			break;
		}

		//�ړI�n��������Ȃ������̂Ō��
		if (increment == 1.0f)
		{
			destinationCandidate = unitPosition - offset;
			break;
		}
	}

	if (valueSum == 0.0f)
		//����ɏ�Q��������������^�[�Q�b�g�Ɍ����Ē��i
		m_pUnit->setTarget(m_pTargetUnit);
	else
		//��Q������������I��
		m_pUnit->setDestination(destinationCandidate, angle, 10);
}

void FlankerController::selectTarget()
{
	auto& sortedUnits = m_pOpponentPlayer->getUnitContainer()->getSortedUnits(UnitStatsValues::Health);
	m_pTargetUnit = sortedUnits.at(0);
}
