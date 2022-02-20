#pragma once
#include <string>
#include <vector>
#include "json.hpp"
#include "Math\Vec3.h"

//ステージ情報クラス
struct Stage
{
	struct UnitInfo
	{
		UnitInfo(const std::string& unitName, const Vec3& position, float angle, int width);

		//管理名
		std::string m_Name;
		//座標
		Vec3 m_Position;
		//角度
		float m_Angle;
		//ユニットの幅
		int m_Width;
	};

	Stage(const nlohmann::json& json);

	//ステージ名
	std::string m_Name;
	//プレイヤーのエネルギー
	int m_PlayerEnergy;

	const std::vector<std::string>& getAvailableUnitList() const;
	const std::vector<UnitInfo>& getUnitList() const;

private:
	//配置可能なユニット名のリスト
	std::vector<std::string> m_AvailableUnitList;
	//ユニットのリスト
	std::vector<UnitInfo> m_UnitList;
};