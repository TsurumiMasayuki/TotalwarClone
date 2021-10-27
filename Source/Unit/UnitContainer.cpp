#include "UnitContainer.h"
#include <algorithm>
#include "Unit\Unit.h"

bool UnitContainer::isSortPredInitialized;
std::unordered_map<UnitStatsValues, std::function<bool(Unit*, Unit*)>> UnitContainer::sortPreds;

void UnitContainer::addUnit(Unit* pUnit)
{
	//���������Ă��Ȃ������珉����
	if (m_SortedUnits.empty())
	{
		m_SortedUnits.emplace(UnitStatsValues::Health, std::vector<Unit*>());
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
}
