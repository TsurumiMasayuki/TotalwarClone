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

	std::string m_AttackName;
	Vec3 m_Position;
};

struct UnitStats
{
public:
	UnitStats(nlohmann::json& json);

	const std::vector<AttackInfo>& getMainAttacks() const;

	//�Ǘ���
	std::string m_Name;

	//�\����
	std::string m_DisplayName;

	//���f����
	std::string m_ModelName;

	//�I�u�W�F�N�g��
	int m_ObjectCount;

	//�I�u�W�F�N�g�̑傫��
	Vec3 m_ObjectSize;

	//���f���̑傫��
	Vec3 m_ModelSize;

	//�I�u�W�F�N�g���̊Ԋu
	float m_SpacePerObject;

	//�I�u�W�F�N�g���Ƃ�HP�̍ő�l
	float m_MaxHealthPerObject;

	//�I�u�W�F�N�g���Ƃ̃V�[���h�̍ő�l
	float m_MaxShieldPerObject;

	//�X�s�[�h
	float m_Speed;

	//��]�X�s�[�h
	float m_RotationSpeed;

	//�f�o�b�O�p�F
	Color m_DebugColor;

	//�f�t�H���g�̐���
	int m_DefaultWidth;

	//���j�b�g�̃R�X�g
	int m_EnergyCost;

private:
	//�U���̃��X�g
	std::vector<AttackInfo> m_AttackInfoList;
};