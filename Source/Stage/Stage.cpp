#include "Stage.h"

Stage::Stage(const nlohmann::json& json)
	: m_Name(json["StageName"]),
	m_PlayerEnergy(json["PlayerEnergy"])
{
	if (json.contains("AvailableUnits"))
	{
		auto& availableUnits = json["AvailableUnits"];
		//�g�p�\���j�b�g���X�g��������Ă���Ȃ�ǂݍ���
		if (!availableUnits.is_null())
		{
			//�g�p�\���j�b�g���X�g�ǂݍ���
			for (auto& unit : availableUnits)
			{
				m_AvailableUnitList.emplace_back((std::string)unit);
			}
		}
	}

	//�G���j�b�g���X�g�ǂݍ���
	for (auto& unit : json["EnemyUnits"])
	{
		std::string name = unit["EnemyName"];
		Vec3 position = Vec3(unit["Position"][0], unit["Position"][1], unit["Position"][2]);
		float angle = unit["Angle"];
		int width = unit["Width"];

		m_UnitList.emplace_back(UnitInfo(name, position, angle, width));
	}
}

const std::vector<std::string>& Stage::getAvailableUnitList() const
{
	return m_AvailableUnitList;
}

const std::vector<Stage::UnitInfo>& Stage::getUnitList() const
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
