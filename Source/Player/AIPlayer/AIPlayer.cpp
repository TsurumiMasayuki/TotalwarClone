#include "AIPlayer.h"
#include "Unit\Unit.h"
#include "AI\Controller\SimpleMoveController.h"
#include "AI\Controller\MarchController.h"
#include "Component\Utility\Transform.h"
#include "GameState.h"

Game::GameState Game::g_GameState;

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

	if (Game::g_GameState == Game::GameState::CombatPhaseBegin)
	{
		Unit* pEnemyCenterUnit = m_pOpponentPlayer->getUnitContainer()->getCenterUnit();

		//�^�񒆂̃��j�b�g���擾
		const auto& units = m_Units.getSortedUnits(UnitStatsValues::Speed);
		auto pBaseUnit = getUnitContainer()->getCenterUnit();

		//�R���g���[���[�𐶐����ēo�^
		m_Controllers.at(pBaseUnit) = new SimpleMoveController(pBaseUnit, pEnemyCenterUnit, pBaseUnit->getUnitStats()->m_DefaultWidth);
		const Vec3& slowestUnitPos = pBaseUnit->getTransform().getLocalPosition();

		//��Ԓx�����j�b�g�ȊO�ɓK�p
		for (auto pUnit : units)
		{
			if (m_Controllers.at(pUnit))
				continue;

			//��ɂ���I�u�W�F�N�g����̑��΍��W���Z�o
			const Vec3& relativePos = pUnit->getTransform().getLocalPosition() - slowestUnitPos;
			//�R���g���[���[�𐶐�
			m_Controllers.at(pUnit) = new MarchController(pUnit, this, relativePos, pUnit->getUnitStats()->m_DefaultWidth);
		}

		//����������
		for (auto& pair : m_Controllers)
		{
			pair.second->start();
		}
	}

	if (Game::g_GameState == Game::GameState::CombatPhase)
	{
		//�X�V����
		for (auto& pair : m_Controllers)
		{
			pair.second->update();
		}
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

int AIPlayer::getTeamID()
{
	return m_TeamID;
}

void AIPlayer::addUnit(Unit* pUnit)
{
	//���j�b�g��ǉ�
	m_Units.addUnit(pUnit);
	//�R���g���[���[�̃X���b�g�ǉ�
	m_Controllers.emplace(pUnit, nullptr);
}

void AIPlayer::removeUnit(Unit* pUnit)
{
	//���j�b�g���폜
	m_Units.removeUnit(pUnit);
}

UnitContainer* AIPlayer::getUnitContainer()
{
	return &m_Units;
}

void AIPlayer::init(int teamNum, IPlayer* pOpponentPlayer, ValueMap* pValueMap)
{
	m_TeamID = teamNum;
	m_pOpponentPlayer = pOpponentPlayer;
	m_pValueMap = pValueMap;
}