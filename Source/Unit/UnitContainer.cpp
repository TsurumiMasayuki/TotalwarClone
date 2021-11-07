#include "UnitContainer.h"
#include <algorithm>

#include "Component\Utility\Transform.h"
#include "Unit\Unit.h"

bool UnitContainer::isSortPredInitialized;
std::unordered_map<UnitStatsValues, std::function<bool(Unit*, Unit*)>> UnitContainer::sortPreds;

void UnitContainer::addUnit(Unit* pUnit)
{
	//���������Ă��Ȃ������珉����
	if (m_SortedUnits.empty())
	{
		m_SortedUnits.emplace(UnitStatsValues::Health, std::vector<Unit*>());
		m_SortedUnits.emplace(UnitStatsValues::Shield, std::vector<Unit*>());
		m_SortedUnits.emplace(UnitStatsValues::ObjectCount, std::vector<Unit*>());
		m_SortedUnits.emplace(UnitStatsValues::Speed, std::vector<Unit*>());
	}

	m_Units.emplace_back(pUnit);

	for (auto& units : m_SortedUnits)
	{
		units.second.emplace_back(pUnit);
	}
}

Unit* UnitContainer::getUnit(int index)
{
	return m_Units.at(index);
}

const std::vector<Unit*>& UnitContainer::getUnits()
{
	return m_Units;
}

const std::vector<Unit*>& UnitContainer::getSortedUnits(UnitStatsValues sortValue)
{
	//�X�V
	updateContainer(sortValue);

	return m_SortedUnits.at(sortValue);
}

Unit* UnitContainer::getCenterUnit()
{
	//���S���W���Z�o
	Vec3 sum;
	for (auto pUnit : m_Units)
	{
		sum += pUnit->getTransform().getLocalPosition();
	}
	Vec3 center = sum / (float)m_Units.size();

	//���Őݒ�
	Unit* pCenterUnit = nullptr;
	float minimumDistance = 100000.0f;
	for (auto pUnit : m_Units)
	{
		const Vec3& pos = pUnit->getTransform().getLocalPosition();
		float distance = center.distance(pos);

		//�ŏ��������X�V
		if (distance < minimumDistance)
		{
			pCenterUnit = pUnit;
			minimumDistance = distance;
		}
	}
	return pCenterUnit;
}

void UnitContainer::updateContainer(UnitStatsValues value)
{
	//�\�[�g�p�֐�������������Ă��Ȃ������珉����
	if (!isSortPredInitialized) 
		initializeSortPreds();

	//�\�[�g����
	auto& sortPred = sortPreds.at(value);
	//�\�[�g�Ώ�
	auto& sortTarget = m_SortedUnits.at(value);
	//�\�[�g����
	std::sort(sortTarget.begin(), sortTarget.end(), sortPred);
}

void UnitContainer::initializeSortPreds()
{
	isSortPredInitialized = true;

	//�̗�
	sortPreds.emplace(UnitStatsValues::Health, 
		[](Unit* a, Unit* b)
		{
			return a->getHealth() < b->getHealth();
		}
	);

	//�V�[���h
	sortPreds.emplace(UnitStatsValues::Shield,
		[](Unit* a, Unit* b)
		{
			return a->getShield() < b->getShield();
		}
	);

	//�X�s�[�h
	sortPreds.emplace(UnitStatsValues::Speed,
		[](Unit* a, Unit* b)
		{
			return a->getUnitStats()->m_Speed < b->getUnitStats()->m_Speed;
		}
	);

	//�I�u�W�F�N�g��
	sortPreds.emplace(UnitStatsValues::ObjectCount,
		[](Unit* a, Unit* b)
		{
			return a->getObjectCount() < b->getObjectCount();
		}
	);
}
