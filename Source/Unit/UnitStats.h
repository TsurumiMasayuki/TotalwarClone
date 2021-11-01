#pragma once
#include <string>
#include <vector>
#include "json.hpp"
#include "Math\Vec3.h"
#include "Utility\Color.h"

enum class UnitStatsValues
{
	Health,
	Shield,
	ObjectCount,
	Speed,
	RotationSpeed
};

struct AttackInfo
{
	AttackInfo(const std::string& attackName, const Vec3& position);

	const std::string m_AttackName;
	const Vec3 m_Position;
};

struct UnitStats
{
public:
	UnitStats(nlohmann::json& json);

	const std::vector<AttackInfo>& getMainAttacks() const;

	//�Ǘ���
	const std::string m_Name;

	//�\����
	const std::string m_DisplayName;

	//�I�u�W�F�N�g��
	const int m_ObjectCount;

	//�I�u�W�F�N�g�̑傫��
	const Vec3 m_ObjectSize;

	//�I�u�W�F�N�g���̊Ԋu
	const float m_SpacePerObject;

	//�I�u�W�F�N�g���Ƃ�HP�̍ő�l
	const float m_MaxHealthPerObject;

	//�I�u�W�F�N�g���Ƃ̃V�[���h�̍ő�l
	const float m_MaxShieldPerObject;

	//�X�s�[�h
	const float m_Speed;

	//��]�X�s�[�h
	const float m_RotationSpeed;

	//�f�o�b�O�p�F
	const Color m_DebugColor;

private:
	//�U���̃��X�g
	std::vector<AttackInfo> m_AttackInfoList;
};