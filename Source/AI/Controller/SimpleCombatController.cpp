#include "SimpleCombatController.h"

#include "Component\Utility\Transform.h"

#include "Unit\UnitContainer.h"

SimpleCombatController::SimpleCombatController(Unit* pUnit, IPlayer* pOpponentPlayer)
	: m_pUnit(pUnit),
	m_pOpponentPlayer(pOpponentPlayer)
{
}

void SimpleCombatController::start()
{
}

void SimpleCombatController::update()
{
	auto pTarget = m_pUnit->getTarget();

	if (pTarget == nullptr)
	{
		searchTarget();
	}
}

void SimpleCombatController::searchTarget()
{
	const std::vector<Unit*>& units = m_pOpponentPlayer->getUnitContainer()->getUnits();
	std::vector<Unit*> sortVector;

	const Vec3& myPos = m_pUnit->getTransform().getLocalPosition();

	//�\�[�g���邽�߃R�s�[����
	std::copy(units.begin(), units.end(), std::back_inserter(sortVector));
	//�R���g���[�����郆�j�b�g�ɋ߂����Ƀ\�[�g
	std::sort(sortVector.begin(), sortVector.end(),
		[myPos](Unit* a, Unit* b)
		{
			const Vec3& posA = a->getTransform().getLocalPosition();
			const Vec3& posB = b->getTransform().getLocalPosition();

			float sqrDistanceA = myPos.sqrDistance(posA);
			float sqrDistanceB = myPos.sqrDistance(posB);

			return sqrDistanceA < sqrDistanceB;
		}
	);

	Unit* pTarget = nullptr;

	for (auto pUnit : sortVector)
	{
		//���j�b�g���S�ł��Ă��Ȃ��Ȃ�^�[�Q�b�g�Ɏw��
		if (pUnit->getObjectCount() != 0)
		{
			pTarget = pUnit;
			break;
		}
	}

	//�^�[�Q�b�g�ݒ�
	m_pUnit->setTarget(pTarget);
}
