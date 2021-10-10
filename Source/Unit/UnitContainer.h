#pragma once
#include <functional>
#include <vector>
#include <unordered_map>
#include "UnitInfo\UnitStats.h"

class Unit;

class UnitContainer
{
public:
	void addUnit(Unit* pUnit);
	Unit* getUnit(int index);

	//値に沿ってソートされたユニットを取得(昇順)
	const std::vector<Unit*>& getSortedUnits(UnitStatsValues sortValue);

private:
	void updateContainer(UnitStatsValues value);
	static void initializeSortPreds();

private:
	static bool isSortPredInitialized;
	static std::unordered_map<UnitStatsValues, std::function<bool(Unit*, Unit*)>> sortPreds;

	std::vector<Unit*> m_Units;
	std::unordered_map<UnitStatsValues, std::vector<Unit*>> m_SortedUnits;
};