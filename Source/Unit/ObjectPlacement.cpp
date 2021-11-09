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

	//��̐��̓��j�b�g�̐���0���Z�ɂȂ�Ȃ��l�͈̔͂Ɏ��܂�悤�ɐ���
	m_Width = MathUtility::clamp(m_Width, 1, objectCount);
	//�؂�グ�p�Ɋ���
	float div = (float)objectCount / (float)m_Width;
	//�؂�グ�čs�̐������߂�
	div = std::ceil(div);

	int xSize = m_Width;
	int zSize = (int)div;

	//�I�u�W�F�N�g�̐��̕�����vector�̃T�C�Y�m��
	results.reserve(objectCount);

	//�c��̌v�Z��
	int remainObjCount = objectCount;
	//�I�u�W�F�N�g���Ƃ̍��W�v�Z
	for (int z = zSize - 1; z >= 0; z--)
	{
		//���������ׂ̈ɔ���
		if (remainObjCount < m_Width)
		{
			xSize = remainObjCount;
		}

		//�x�[�X�̍��W���v�Z
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
	//���W��K�p
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
