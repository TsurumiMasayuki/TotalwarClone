#include "UnitStats.h"
#include "Utility\StringUtility.h"

UnitStats::UnitStats(nlohmann::json& json)
	:  m_Name(json["UnitStats"]["Name"]),
	m_DisplayName(json["UnitStats"]["DisplayName"]),
	m_ModelName(json["UnitStats"]["ModelName"]),
	m_ObjectCount((int)json["UnitStats"]["ObjectCount"]),
	m_ObjectSize(Vec3((float)json["UnitStats"]["ObjectSize"][0],
		(float)json["UnitStats"]["ObjectSize"][1],
		(float)json["UnitStats"]["ObjectSize"][2])),
	m_ModelSize(Vec3((float)json["UnitStats"]["ModelSize"][0],
		(float)json["UnitStats"]["ModelSize"][1],
		(float)json["UnitStats"]["ModelSize"][2])),
	m_SpacePerObject((float)json["UnitStats"]["SpacePerObject"]),
	m_MaxHealthPerObject((float)json["UnitStats"]["MaxHealthPerObject"]),
	m_MaxShieldPerObject((float)json["UnitStats"]["MaxShieldPerObject"]),
	m_Speed((float)json["UnitStats"]["Speed"]),
	m_RotationSpeed((float)json["UnitStats"]["RotationSpeed"]),
	m_DebugColor(Color((float)json["UnitStats"]["DebugColor"][0],
		(float)json["UnitStats"]["DebugColor"][1],
		(float)json["UnitStats"]["DebugColor"][2],
		1.0f)),
	m_DefaultWidth((int)json["UnitStats"]["DefaultUnitWidth"]),
	m_EnergyCost((int)json["UnitStats"]["EnergyCost"])
{
	//攻撃の情報を読み取る
	for (auto& attackInfo : json["MainAttacks"])
	{
		auto& position = attackInfo["Position"];
		Vec3 vecPosition = Vec3((float)position[0], (float)position[1], (float)position[2]);
		m_AttackInfoList.emplace_back(AttackInfo(attackInfo["AttackName"], vecPosition));
	}
}

const std::vector<AttackInfo>& UnitStats::getMainAttacks() const
{
	return m_AttackInfoList;
}

AttackInfo::AttackInfo(const std::string& attackName, const Vec3& position)
	: m_AttackName(attackName),
	m_Position(position)
{
}
