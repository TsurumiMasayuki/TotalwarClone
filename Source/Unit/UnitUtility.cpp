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

	//��̐��̓��j�b�g�̐���0���Z�ɂȂ�Ȃ��l�͈̔͂Ɏ��܂�悤�ɐ���
	int clampedUnitWidth = MathUtility::clamp(unitWidth, 1, objectCount);
	//�؂�グ�p�Ɋ���
	float div = (float)objectCount / (float)clampedUnitWidth;
	//�؂�グ�čs�̐������߂�
	div = std::ceil(div);

	int xSize = clampedUnitWidth;
	int zSize = (int)div;

	//�I�u�W�F�N�g�̐��̕�����vector�̃T�C�Y�m��
	results.reserve(objectCount);

	//�c��̌v�Z��
	int remainObjCount = objectCount;
	//�I�u�W�F�N�g���Ƃ̍��W�v�Z
	for (int z = zSize - 1; z >= 0; z--)
	{
		//���������ׂ̈ɔ���
		if (remainObjCount < clampedUnitWidth)
		{
			xSize = remainObjCount;
		}

		//�x�[�X�̍��W���v�Z
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