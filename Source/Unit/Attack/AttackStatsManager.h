#pragma once
#include <string>
#include <unordered_map>

#include "Unit\Attack\AttackStats.h"
#include "Utility\SingletonTemplate.h"

//攻撃ステータス管理クラス
class AttackStatsManager
	: public SingletonTemplate<AttackStatsManager>
{
public:
	friend class SingletonTemplate<AttackStatsManager>;

	//ファイルの読み込み
	void load(const std::string& key, const std::string& filePath);
	//ステータスの取得
	const AttackStats& getAttackStats(const std::string& key) const;

protected:
	//コンストラクタを隠す
	AttackStatsManager() {};
	//デストラクタを隠す
	virtual ~AttackStatsManager() {};

private:
	std::unordered_map<std::string, AttackStats> m_StatsList;
};