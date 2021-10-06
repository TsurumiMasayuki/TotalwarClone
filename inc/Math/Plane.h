#pragma once
#include <array>
#include <Math\Vec3.h>

class Plane
{
public:
	Plane(Vec3 p1, Vec3 p2, Vec3 p3);

	//平面の法線
	const Vec3& getNormal() const;
	//pointが平面の表側と裏側どちらにいるか返す(trueは表、falseは裏)
	bool getSide(const Vec3& point) const;
	//線分と交差させる
	bool lineTest(const Vec3& p1, const Vec3& p2, Vec3& hitPos) const;

private:
	//平面を構成する三点
	std::array<Vec3, 3> m_Points;
	//法線
	Vec3 m_Normal;
};