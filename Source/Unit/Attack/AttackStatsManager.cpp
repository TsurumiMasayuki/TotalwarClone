#include "AttackStatsManager.h"
#include <fstream>
#include "json.hpp"

void AttackStatsManager::load(const std::string& key, const std::string& filePath)
{
	//�t�@�C���ǂݍ���
	std::ifstream ifs(filePath);
	bool hoge = ifs.is_open();

	//json�֕ϊ�
	nlohmann::json json;
	ifs >> json;
	ifs.close();

	//�X�e�[�^�X�ǂݍ���
	AttackStats stats(
		(float)json["Damage"],
		(float)json["AttackInterval"],
		(float)json["AttackRange"],
		(float)json["TrackSpeed"],
		(bool)json["IsTrackTarget"],
		(int)json["MaxAttackCount"],
		(bool)json["IsInfiniteAttack"]
	);

	//�U���N���X��o�^
	m_StatsList.emplace(key, stats);
}

const AttackStats& AttackStatsManager::getAttackStats(const std::string& key) const
{
	return m_StatsList.at(key);
}
