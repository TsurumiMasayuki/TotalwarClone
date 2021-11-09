#include "ObjectPlacement.h"
#include <DirectXMath.h>
#include "Component\Utility\Transform.h"
#include "Math\MathUtility.h"

ObjectPlacement::ObjectPlacement()
	: m_BasePos(0.0f),
	m_Width(1),
	m_SpacePerObject(1.0f),
	m_Radian(0.0f)
{
}

void ObjectPlacement::calculateObjectPositions(std::vector<Vec3>& results)
{
	DirectX::XMMATRIX rotateMat = DirectX::XMMatrixRotationRollPitchYaw(0.0f, m_Radian, 0.0f);

	int objectCount = (int)m_ObjectList.size();

	//列の数はユニットの数と0除算にならない値の範囲に収まるように制限
	m_Width = MathUtility::clamp(m_Width, 1, objectCount);
	//切り上げ用に割る
	float div = (float)objectCount / (float)m_Width;
	//切り上げて行の数を求める
	div = std::ceil(div);

	int xSize = m_Width;
	int zSize = (int)div;

	//オブジェクトの数の分だけvectorのサイズ確保
	results.reserve(objectCount);

	//残りの計算回数
	int remainObjCount = objectCount;
	//オブジェクトごとの座標計算
	for (int z = zSize - 1; z >= 0; z--)
	{
		//中央揃えの為に判定
		if (remainObjCount < m_Width)
		{
			xSize = remainObjCount;
		}

		//ベースの座標を計算
		Vec3 rotateOffset(-m_SpacePerObject * (float)(xSize - 1) * 0.5f, 0.0f, -m_SpacePerObject * (float)(zSize) * 0.5f);
		for (int x = 0; x < xSize; x++)
		{
			Vec3 newPosition = Vec3(m_SpacePerObject * x, 0.0f, m_SpacePerObject * z) + rotateOffset;
			newPosition = newPosition.multMatrix(rotateMat) + m_BasePos;
			results.emplace_back(newPosition);
			remainObjCount--;
		}
	}
}

void ObjectPlacement::applyObjectPositions()
{
	std::vector<Vec3> positions;
	calculateObjectPositions(positions);

	int index = 0;
	//座標を適用
	for (auto pObject : m_ObjectList)
	{
		pObject->setLocalPosition(positions.at(index));
		index++;
	}
}

void ObjectPlacement::addObject(Transform* pObject)
{
	m_ObjectList.insert(pObject);
}

void ObjectPlacement::removeObject(Transform* pObject)
{
	m_ObjectList.erase(pObject);
}

void ObjectPlacement::setBasePos(const Vec3& basePos)
{
	m_BasePos = basePos;
}

const Vec3& ObjectPlacement::getBasePos() const
{
	return m_BasePos;
}

void ObjectPlacement::setWidth(int width)
{
	m_Width = width;
}

int ObjectPlacement::getWidth() const
{
	return m_Width;
}

void ObjectPlacement::setSpacePerObject(float spacePerObject)
{
	m_SpacePerObject = spacePerObject;
}

float ObjectPlacement::getSpacePerObject() const
{
	return m_SpacePerObject;
}

void ObjectPlacement::setAngle(float angle)
{
	m_Radian = MathUtility::toRadian(angle);
}

float ObjectPlacement::getAngle() const
{
	return MathUtility::toDegree(m_Radian);
}
