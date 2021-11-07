#pragma once
#include <vector>
#include "Math\Vec3.h"

struct UnitStats;

namespace UnitUtility
{
	void calculateObjectPositions(UnitStats* pUnitStats, std::vector<Vec3>& results, const Vec3& destination, float radian, int unitWidth, int objectCount);
}