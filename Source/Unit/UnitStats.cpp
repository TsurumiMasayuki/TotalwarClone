#include "UnitStats.h"

UnitStats::UnitStats(nlohmann::json& json)
	:  m_Name(json["Name"]),
	m_DisplayName(json["DisplayName"]),
	m_ObjectCount((int)json["ObjectCount"]),
	m_ObjectSize(Vec3((float)json["ObjectSize"][0], (float)json["ObjectSize"][1], (float)json["ObjectSize"][2])),
	m_SpacePerObject((float)json["SpacePerObject"]),
	m_MaxHealthPerObject((float)json["MaxHealthPerObject"]),
	m_MaxShieldPerObject((float)json["MaxShieldPerObject"]),
	m_Speed((float)json["Speed"]),
	m_RotationSpeed((float)json["RotationSpeed"])
{
}
