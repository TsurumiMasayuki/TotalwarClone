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

	const std::string m_AttackName;
	const Vec3 m_Position;
};

struct UnitStats
{
public:
	UnitStats(nlohmann::json& json);

	const std::vector<AttackInfo>& getMainAttacks() const;

	//管理名
	const std::string m_Name;

	//表示名
	const std::wstring m_DisplayName;

	//モデル名
	const std::string m_ModelName;

	//オブジェクト数
	const int m_ObjectCount;

	//オブジェクトの大きさ
	const Vec3 m_ObjectSize;

	//モデルの大きさ
	const Vec3 m_ModelSize;

	//オブジェクト毎の間隔
	const float m_SpacePerObject;

	//オブジェクトごとのHPの最大値
	const float m_MaxHealthPerObject;

	//オブジェクトごとのシールドの最大値
	const float m_MaxShieldPerObject;

	//スピード
	const float m_Speed;

	//回転スピード
	const float m_RotationSpeed;

	//デバッグ用色
	const Color m_DebugColor;

	//デフォルトの整列幅
	const int m_DefaultWidth;

	//ユニットのコスト
	const int m_EnergyCost;

private:
	//攻撃のリスト
	std::vector<AttackInfo> m_AttackInfoList;
};