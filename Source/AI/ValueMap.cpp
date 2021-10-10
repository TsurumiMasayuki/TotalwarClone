#include "ValueMap.h"

ValueMap::ValueMap()
{
	m_ValueMaps.emplace(UnitStatsValues::Health, std::vector<Value>());
}

void ValueMap::writeMap(UnitStatsValues type, const Value& value)
{
	m_ValueMaps.at(type).emplace_back(value);
}

float ValueMap::readMap(UnitStatsValues type, const Vec3& readPosition)
{
	float sum = 0.0f;
	for (auto& value : m_ValueMaps.at(type))
	{
		Vec3 diff = readPosition - value.m_Center;
		if (diff.length() < value.m_Size)
		{
			sum += value.m_Value;
		}
	}

	return sum;
}

void ValueMap::clear(UnitStatsValues type)
{
	m_ValueMaps.at(type).clear();
}

void ValueMap::clearAll()
{
	for (auto& valueMap : m_ValueMaps)
	{
		valueMap.second.clear();
	}
}

const std::unordered_map<UnitStatsValues, std::vector<Value>>& ValueMap::getValueMaps() const
{
	return m_ValueMaps;
}
