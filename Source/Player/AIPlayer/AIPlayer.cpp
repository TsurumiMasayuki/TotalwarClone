#include "AIPlayer.h"
#include "Component\Utility\Transform.h"

#include "GameState.h"

#include "AI\Controller\SimpleMoveController.h"
#include "AI\Controller\SimpleCombatController.h"
#include "AI\Controller\MarchController.h"

#include "Unit\Unit.h"

Game::GameState Game::g_GameState;

AIPlayer::AIPlayer()
{
}

AIPlayer::~AIPlayer()
{
}

void AIPlayer::onStart()
{
	m_ControllerUpdated = false;
}

void AIPlayer::onUpdate()
{
	//�ΐ푊�肪���Ȃ��Ȃ���s���Ȃ�
	if (m_pOpponentPlayer == nullptr) return;

	if (Game::g_GameState == Game::GameState::CombatPhaseBegin)
	{
		Unit* pEnemyCenterUnit = m_pOpponentPlayer->getUnitContainer()->getCenterUnit();

		//��Ԓx�����j�b�g���擾
		const auto& units = m_Units.getSortedUnits(UnitStatsValues::Speed);
		m_pBaseUnit = units.at(0);

		//�R���g���[���[�𐶐����ēo�^
		m_Controllers.at(m_pBaseUnit) = new SimpleMoveController(m_pBaseUnit, pEnemyCenterUnit, m_pBaseUnit->getUnitStats()->m_DefaultWidth);
		const Vec3& slowestUnitPos = m_pBaseUnit->getTransform().getLocalPosition();

		//��Ԓx�����j�b�g�ȊO�ɓK�p
		for (auto pUnit : units)
		{
			if (m_Controllers.at(pUnit))
				continue;

			//��ɂ���I�u�W�F�N�g����̑��΍��W���Z�o
			const Vec3& relativePos = pUnit->getTransform().getLocalPosition() - slowestUnitPos;
			//�R���g���[���[�𐶐�
			m_Controllers.at(pUnit) = new MarchController(pUnit, m_pBaseUnit, relativePos, pUnit->getUnitStats()->m_DefaultWidth);
		}

		//����������
		for (auto& pair : m_Controllers)
		{
			pair.second->start();
		}
	}

	if (Game::g_GameState == Game::GameState::CombatPhase)
	{
		//���j�b�g���퓬���Ȃ�R���g���[���[���X�V
		for (auto pUnit : m_Units.getUnits())
		{
			//�ړ��̒��S�ɂȂ郆�j�b�g�����S���Ă�����퓬AI�ɐ؂�ւ���
			if (!pUnit->isInCombat() &&
				m_pBaseUnit->getObjectCount() > 0) continue;

			//�퓬���Ȃ�continue
			if (m_ControllerUpdated) continue;

			for (auto pUnit2 : m_Units.getUnits())
			{
				setNewController(pUnit2, new SimpleCombatController(pUnit2, m_pOpponentPlayer));
			}

			m_ControllerUpdated = true;
		}

		//�X�V����
		for (auto& pair : m_Controllers)
		{
			pair.second->update();
		}
	}
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

bool AIPlayer::isDefeat()
{
	bool isDefeat = true;

	const auto& units = m_Units.getUnits();

	for (const auto pUnit : units)
	{
		if (pUnit->getObjectCount() == 0) continue;

		isDefeat = false;
		break;
	}

	return isDefeat;
}

void AIPlayer::setNewController(Unit* pUnit, AbstractController* pController)
{
	//null�łȂ��Ȃ�delete
	if (m_Controllers.at(pUnit) != nullptr)
		delete m_Controllers.at(pUnit);

	//�V�����R���g���[���[���Z�b�g
	m_Controllers.at(pUnit) = pController;
}
