#pragma once

enum class UnitStatsValues
{
	Health,
	Shield,
	UnitCount,
	Speed,
	RotationSpeed
};

struct UnitStats
{
	//�I�u�W�F�N�g��
	int m_ObjectCount;

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
};