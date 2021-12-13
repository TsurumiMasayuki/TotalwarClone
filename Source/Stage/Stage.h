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
		const std::string m_Name;
		//座標
		const Vec3 m_Position;
		//角度
		const float m_Angle;
		//ユニットの幅
		const int m_Width;
	};

	Stage(const nlohmann::json& json);

	//ステージ名
	const std::string m_Name;
	//プレイヤーのエネルギー
	const int m_PlayerEnergy;

	const std::vector<std::string>& getAvailableUnitList() const;
	const std::vector<UnitInfo>& getUnitList() const;

private:
	//配置可能なユニット名のリスト
	std::vector<std::string> m_AvailableUnitList;
	//ユニットのリスト
	std::vector<UnitInfo> m_UnitList;
};