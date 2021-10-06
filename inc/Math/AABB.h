#pragma once
#include <Math\Vec3.h>

class AABB3D
{
public:
	AABB3D() : AABB3D(0, 0, 0, 0, 0, 0)
	{
	}

	AABB3D(float x, float y, float z, float halfSizeX, float halfSizeY, float halfSizeZ)
		: m_Left(x - halfSizeX),
		m_Right(x + halfSizeX),
		m_Down(y - halfSizeY),
		m_Up(y + halfSizeY),
		m_Back(z - halfSizeZ),
		m_Front(z + halfSizeZ)	
	{
	}

	void setParams(float x, float y, float z, float halfSizeX, float halfSizeY, float halfSizeZ)
	{
		m_Left = x - halfSizeX;
		m_Right = x + halfSizeX;
		m_Down = y - halfSizeY;
		m_Up = y + halfSizeY;
		m_Back = z - halfSizeZ;
		m_Front = z + halfSizeZ;
	}

	float getLeft() const { return m_Left; }
	float getRight() const { return m_Right; }

	float getDown()	const { return m_Down; }
	float getUp() const { return m_Up; }

	float getBack()	const { return m_Down; }
	float getFront() const { return m_Up; }
	
private:
	float m_Left;
	float m_Right;

	float m_Down;
	float m_Up;

	float m_Back;
	float m_Front;
};