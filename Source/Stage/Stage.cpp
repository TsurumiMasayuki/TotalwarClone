#include "Stage.h"

Stage::Stage(const nlohmann::json& json)
	: m_Name(json["StageName"]),
	m_PlayerEnergy(json["PlayerEnergy"])
{
	//敵ユニットリスト読み込み
	for (auto& unit : json["EnemyUnits"])
	{
		std::string name = unit["EnemyName"];
		Vec3 position = Vec3(unit["Position"][0], unit["Position"][1], unit["Position"][2]);
		float angle = unit["Angle"];
		int width = unit["Width"];

		m_UnitList.emplace_back(UnitInfo(name, position, angle, width));
	}
}

const std::vector<Stage::UnitInfo> Stage::getUnitList() const
{
	return m_UnitList;
}

Stage::UnitInfo::UnitInfo(const std::string& unitName, const Vec3& position, float angle, int width)
	: m_Name(unitName),
	m_Position(position),
	m_Angle(angle),
	m_Width(width)
{
}
