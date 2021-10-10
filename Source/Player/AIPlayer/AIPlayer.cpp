#include "AIPlayer.h"
#include "Unit\Unit.h"

AIPlayer::AIPlayer()
{
}

AIPlayer::~AIPlayer()
{
}

void AIPlayer::onStart()
{
}

void AIPlayer::onUpdate()
{
	//�ΐ푊�肪���Ȃ��Ȃ���s���Ȃ�
	if (m_pOpponentPlayer == nullptr) return;

	auto& sortedUnits = m_pOpponentPlayer->getUnits()->getSortedUnits(UnitStatsValues::Health);
	auto pTarget = sortedUnits.at(0);
	auto pMyUnit = m_Units.getUnit(0);

	Vec3 unitPosition = pMyUnit->getTransform().getLocalPosition();
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

		float value = m_pValueMap->readMap(UnitStatsValues::Health, unitPosition + rotateOffset);
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
		pMyUnit->setTarget(pTarget);
	else
		//��Q������������I��
		pMyUnit->setDestination(destinationCandidate, angle);
}

void AIPlayer::addUnit(Unit* pUnit)
{
	m_Units.addUnit(pUnit);
}

UnitContainer* AIPlayer::getUnits()
{
	return &m_Units;
}

void AIPlayer::init(int teamNum, IPlayer* pOpponentPlayer, ValueMap* pValueMap)
{
	m_TeamNum = teamNum;
	m_pOpponentPlayer = pOpponentPlayer;
	m_pValueMap = pValueMap;
}