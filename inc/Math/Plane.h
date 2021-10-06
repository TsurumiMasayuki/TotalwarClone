#pragma once
#include <array>
#include <Math\Vec3.h>

class Plane
{
public:
	Plane(Vec3 p1, Vec3 p2, Vec3 p3);

	//���ʂ̖@��
	const Vec3& getNormal() const;
	//point�����ʂ̕\���Ɨ����ǂ���ɂ��邩�Ԃ�(true�͕\�Afalse�͗�)
	bool getSide(const Vec3& point) const;
	//�����ƌ���������
	bool lineTest(const Vec3& p1, const Vec3& p2, Vec3& hitPos) const;

private:
	//���ʂ��\������O�_
	std::array<Vec3, 3> m_Points;
	//�@��
	Vec3 m_Normal;
};