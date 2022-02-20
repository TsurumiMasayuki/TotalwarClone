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

	std::string m_AttackName;
	Vec3 m_Position;
};

struct UnitStats
{
public:
	UnitStats(nlohmann::json& json);

	const std::vector<AttackInfo>& getMainAttacks() const;

	//管理名
	std::string m_Name;

	//表示名
	std::string m_DisplayName;

	//モデル名
	std::string m_ModelName;

	//オブジェクト数
	int m_ObjectCount;

	//オブジェクトの大きさ
	Vec3 m_ObjectSize;

	//モデルの大きさ
	Vec3 m_ModelSize;

	//オブジェクト毎の間隔
	float m_SpacePerObject;

	//オブジェクトごとのHPの最大値
	float m_MaxHealthPerObject;

	//オブジェクトごとのシールドの最大値
	float m_MaxShieldPerObject;

	//スピード
	float m_Speed;

	//回転スピード
	float m_RotationSpeed;

	//デバッグ用色
	Color m_DebugColor;

	//デフォルトの整列幅
	int m_DefaultWidth;

	//ユニットのコスト
	int m_EnergyCost;

private:
	//攻撃のリスト
	std::vector<AttackInfo> m_AttackInfoList;
};