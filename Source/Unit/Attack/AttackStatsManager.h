#pragma once
#include <string>
#include <unordered_map>

#include "Unit\Attack\AttackStats.h"
#include "Utility\SingletonTemplate.h"

//�U���X�e�[�^�X�Ǘ��N���X
class AttackStatsManager
	: public SingletonTemplate<AttackStatsManager>
{
public:
	friend class SingletonTemplate<AttackStatsManager>;

	//�t�@�C���̓ǂݍ���
	void load(const std::string& key, const std::string& filePath);
	//�X�e�[�^�X�̎擾
	const AttackStats& getAttackStats(const std::string& key) const;

protected:
	//�R���X�g���N�^���B��
	AttackStatsManager() {};
	//�f�X�g���N�^���B��
	virtual ~AttackStatsManager() {};

private:
	std::unordered_map<std::string, AttackStats> m_StatsList;
};