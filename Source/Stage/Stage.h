#pragma once
#include <string>
#include <vector>
#include "json.hpp"
#include "Math\Vec3.h"

//�X�e�[�W���N���X
struct Stage
{
	struct UnitInfo
	{
		UnitInfo(const std::string& unitName, const Vec3& position, float angle, int width);

		//�Ǘ���
		const std::string m_Name;
		//���W
		const Vec3 m_Position;
		//�p�x
		const float m_Angle;
		//���j�b�g�̕�
		const int m_Width;
	};

	Stage(const nlohmann::json& json);

	//�X�e�[�W��
	const std::string m_Name;
	//�v���C���[�̃G�l���M�[
	const int m_PlayerEnergy;

	const std::vector<std::string>& getAvailableUnitList() const;
	const std::vector<UnitInfo>& getUnitList() const;

private:
	//�z�u�\�ȃ��j�b�g���̃��X�g
	std::vector<std::string> m_AvailableUnitList;
	//���j�b�g�̃��X�g
	std::vector<UnitInfo> m_UnitList;
};