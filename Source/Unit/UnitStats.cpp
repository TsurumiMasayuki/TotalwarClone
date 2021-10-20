#include "UnitStats.h"

UnitStats::UnitStats(nlohmann::json& json)
	:  m_Name(json["UnitStats"]["Name"]),
	m_DisplayName(json["UnitStats"]["DisplayName"]),
	m_ObjectCount((int)json["UnitStats"]["ObjectCount"]),
	m_ObjectSize(Vec3((float)json["UnitStats"]["ObjectSize"][0],
		(float)json["UnitStats"]["ObjectSize"][1],
		(float)json["UnitStats"]["ObjectSize"][2])),
	m_SpacePerObject((float)json["UnitStats"]["SpacePerObject"]),
	m_MaxHealthPerObject((float)json["UnitStats"]["MaxHealthPerObject"]),
	m_MaxShieldPerObject((float)json["UnitStats"]["MaxShieldPerObject"]),
	m_Speed((float)json["UnitStats"]["Speed"]),
	m_RotationSpeed((float)json["UnitStats"]["RotationSpeed"])
{
	//çUåÇÇÃèÓïÒÇì«Ç›éÊÇÈ
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
