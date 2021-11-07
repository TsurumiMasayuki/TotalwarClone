#include "UnitUtility.h"
#include <cmath>
#include <DirectXMath.h>

#include "Math\MathUtility.h"

void UnitUtility::calculateObjectPositions(UnitStats* pUnitStats,
	std::vector<Vec3>& results,
	const Vec3& destination,
	float radian,
	int unitWidth,
	int objectCount)
{
	DirectX::XMMATRIX rotateMat = DirectX::XMMatrixRotationRollPitchYaw(0.0f, radian, 0.0f);

	//列の数はユニットの数と0除算にならない値の範囲に収まるように制限
	int clampedUnitWidth = MathUtility::clamp(unitWidth, 1, objectCount);
	//切り上げ用に割る
	float div = (float)objectCount / (float)clampedUnitWidth;
	//切り上げて行の数を求める
	div = std::ceil(div);

	int xSize = clampedUnitWidth;
	int zSize = (int)div;

	//オブジェクトの数の分だけvectorのサイズ確保
	results.reserve(objectCount);

	//残りの計算回数
	int remainObjCount = objectCount;
	//オブジェクトごとの座標計算
	for (int z = zSize - 1; z >= 0; z--)
	{
		//中央揃えの為に判定
		if (remainObjCount < clampedUnitWidth)
		{
			xSize = remainObjCount;
		}

		//ベースの座標を計算
		//Vec3 basePos(-m_SpacePerObject * (float)(xSize - 1) * 0.5f, -10.0f, -m_SpacePerObject * (float)(zSize) * 0.5f);
		//for (int x = 0; x < xSize; x++)
		//{
		//	Vec3 newPosition = Vec3(m_SpacePerObject * x, 0.0f, m_SpacePerObject * z) + basePos;
		//	newPosition = newPosition.multMatrix(rotateMat) + destination;
		//	results.emplace_back(newPosition);
		//	remainObjCount--;
		//}
	}
}