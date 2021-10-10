#pragma once

enum class UnitStatsValues
{
	Health,
	Shield,
	Armor,
	DefencePower,
};

struct UnitStats
{
	//�I�u�W�F�N�g���Ƃ�HP�̍ő�l
	float m_MaxHealthPerObject;

	//�A�[�}�[
	float m_Armor;

	//�ߐڍU��������
	int m_MeleeAttack;
	//�ߐږh��m��
	int m_MeleeDefence;

	//�ߐڍU���_���[�W
	float m_MeleeDamage;

	//�U���Ԋu
	float m_AttackInterval;
};