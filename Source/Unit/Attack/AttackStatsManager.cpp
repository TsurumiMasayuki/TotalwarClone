#include "AttackStatsManager.h"
#include <fstream>
#include "json.hpp"

void AttackStatsManager::load(const std::string& key, const std::string& filePath)
{
	//ファイル読み込み
	std::ifstream ifs(filePath);
	bool hoge = ifs.is_open();

	//jsonへ変換
	nlohmann::json json;
	ifs >> json;
	ifs.close();

	//ステータス読み込み
	AttackStats stats(
		(float)json["Damage"],
		(float)json["AttackInterval"],
		(float)json["AttackRange"],
		(float)json["TrackSpeed"],
		(bool)json["IsTrackTarget"],
		(int)json["MaxAttackCount"],
		(bool)json["IsInfiniteAttack"]
	);

	//攻撃クラスを登録
	m_StatsList.emplace(key, stats);
}

const AttackStats& AttackStatsManager::getAttackStats(const std::string& key) const
{
	return m_StatsList.at(key);
}
