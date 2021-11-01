#include "AIPlayer.h"
#include "Unit\Unit.h"
#include "AI\Controller\SimpleMoveController.h"
#include "AI\Controller\MarchController.h"

AIPlayer::AIPlayer()
{
}

AIPlayer::~AIPlayer()
{
}

void AIPlayer::onStart()
{
	const auto& opponentUnits = m_pOpponentPlayer->getUnitContainer()->getUnits();

	//��Ԓx�����j�b�g��������
	const auto& units = m_Units.getSortedUnits(UnitStatsValues::Speed);
	auto pSlowestUnit = units.at(0);

	//�R���g���[���[�𐶐����ēo�^
	m_Controllers.at(pSlowestUnit) = new SimpleMoveController(pSlowestUnit, opponentUnits.at(0), 10);

	//��Ԓx�����j�b�g�ȊO�ɓK�p
	for (int i = 1; i < (int)units.size(); i++)
	{
		//�����ʒu�𑊑΍��W�Ƃ��Ď擾
		const Vec3& relativePos = units.at(i)->getTransform().getLocalPosition();
		//�R���g���[���[�𐶐�
		m_Controllers.at(units.at(i)) = new MarchController(units.at(i), this, relativePos, 5);
	}

	//����������
	for (auto& pair : m_Controllers)
	{
		pair.second->start();
	}
}

void AIPlayer::onUpdate()
{
	//�ΐ푊�肪���Ȃ��Ȃ���s���Ȃ�
	if (m_pOpponentPlayer == nullptr) return;

	//�X�V����
	for (auto& pair : m_Controllers)
	{
		pair.second->update();
	}

	//auto& sortedUnits = m_pOpponentPlayer->getUnitContainer()->getSortedUnits(UnitStatsValues::Health);
	//auto pTarget = sortedUnits.at(0);
	//auto pMyUnit = m_Units.getUnit(0);

	//Vec3 unitPosition = pMyUnit->getTransform().getLocalPosition();
	//Vec3 offset = Vec3(50.0f, 0.0f, 0.0f);
	//Vec3 destinationCandidate;

	//float increment = 0.0f;
	//float valueSum = 0.0f;
	//float angle = 0.0f;

	////�ړI�n��������܂Ń��[�v
	//while (true)
	//{
	//	float sin = std::sinf(DirectX::XM_PI * increment);
	//	increment += 0.5f;

	//	//�I�t�Z�b�g����]
	//	Vec3 rotateOffset = Vec3(
	//		offset.x + offset.z * std::cosf(DirectX::XM_PI * 0.5f * sin),
	//		0.0f,
	//		offset.z + offset.x * std::sinf(DirectX::XM_PI * 0.5f * sin)
	//	);

	//	float value = m_pValueMap->readMap(UnitStatsValues::Health, unitPosition + rotateOffset);
	//	valueSum += value;

	//	//�ړI�n��������
	//	if (value < 100.0f)
	//	{
	//		destinationCandidate = unitPosition + rotateOffset;
	//		angle = 90.0f + 180.0f * sin;
	//		break;
	//	}

	//	//�ړI�n��������Ȃ������̂Ō��
	//	if (increment == 1.0f)
	//	{
	//		destinationCandidate = unitPosition - offset;
	//		break;
	//	}
	//}

	//if (valueSum == 0.0f)
	//	//����ɏ�Q��������������^�[�Q�b�g�Ɍ����Ē��i
	//	pMyUnit->setTarget(pTarget);
	//else
	//	//��Q������������I��
	//	pMyUnit->setDestination(destinationCandidate, angle, 10);
}

void AIPlayer::onDestroy()
{
	for (auto pair : m_Controllers)
	{
		delete pair.second;
	}
}

void AIPlayer::addUnit(Unit* pUnit)
{
	//���j�b�g��ǉ�
	m_Units.addUnit(pUnit);
	//�R���g���[���[�̃X���b�g�ǉ�
	m_Controllers.emplace(pUnit, nullptr);
}

UnitContainer* AIPlayer::getUnitContainer()
{
	return &m_Units;
}

void AIPlayer::init(int teamNum, IPlayer* pOpponentPlayer, ValueMap* pValueMap)
{
	m_TeamNum = teamNum;
	m_pOpponentPlayer = pOpponentPlayer;
	m_pValueMap = pValueMap;
}