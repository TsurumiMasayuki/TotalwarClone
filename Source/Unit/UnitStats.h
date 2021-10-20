#pragma once
#include "Math\Vec3.h"

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
	//オブジェクト数
	int m_ObjectCount;

	//オブジェクトの大きさ
	Vec3 m_ObjectSize;

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
};