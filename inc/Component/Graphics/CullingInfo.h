#pragma once
#include <Math\Vec3.h>

struct CullingInfo
{
	CullingInfo(const Vec3& min, const Vec3& max)
		: m_Min(min), m_Max(max)
	{}

	const Vec3& getMin() const { return m_Min; }
	const Vec3& getMax() const { return m_Max; }

private:
	Vec3 m_Min;
	Vec3 m_Max;
};