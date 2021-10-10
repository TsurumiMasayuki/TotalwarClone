#pragma once
#include <vector>
#include <unordered_map>
#include <numeric>
#include "Math\Vec3.h"
#include "UnitInfo\UnitStats.h"

//情報マップに書き込む数値の種類
struct Value
{
	Value(const Vec3& center, float size, float value) 
		: m_Center(center),
		m_Size(size),
		m_Value(value)
	{

	}

	Vec3 m_Center;
	float m_Size;
	float m_Value;
};

class ValueMap
{
public:
	ValueMap();

	void writeMap(UnitStatsValues type, const Value& value);
	float readMap(UnitStatsValues type, const Vec3& readPosition);
	void clear(UnitStatsValues type);
	void clearAll();

	const std::unordered_map<UnitStatsValues, std::vector<Value>>& getValueMaps() const;

private:
	std::unordered_map<UnitStatsValues, std::vector<Value>> m_ValueMaps;
};
