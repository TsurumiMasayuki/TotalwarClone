#include "Unit.h"

#include <algorithm>

#include "Actor\Base\GameObject.h"
#include "Component\Physics\SphereColliderBt.h"
#include "Math\MathUtility.h"

#include "AI\ValueMap.h"
#include "Component\Box2D\CircleColliderB2.h"
#include "Unit\UnitObject.h"
#include "Unit\UnitRenderHelper.h"

//TODO:��]��񎟌����W�p�ɕς���

void Unit::onStart()
{
}

void Unit::onUpdate()
{
	m_StateLockTimer.update();

	//�C���X�^���V���O�p���𑗂�
	std::vector<DirectX::XMMATRIX> objMatrices;
	for (auto pUnitObject : m_UnitObjects)
	{
		objMatrices.emplace_back(pUnitObject->getTransform().getWorldMatrix());
	}
	m_pRenderHelper->appendInstanceInfo(objMatrices);

	int aliveCount = 0;
	//�����Ă���I�u�W�F�N�g���J�E���g
	for (auto pUnitObject : m_UnitObjects)
	{
		if (pUnitObject->getState() != UnitObject::State::Dead)
			aliveCount++;
	}

	//�����Ă���I�u�W�F�N�g�����Ȃ��Ȃ�return
	if (aliveCount == 0) return;

	updateCenterPosition();

	//�U���w��������Ȃ�^�[�Q�b�g�Ɍ����Ĉړ�
	if (m_pTargetUnit != nullptr)
	{
		const Vec3& pos = getTransform().getLocalPosition();
		const Vec3& destination = m_pTargetUnit->getTransform().getLocalPosition();
		Vec3 diff = destination - pos;
		diff.y = 0.0f;
		//�^�[�Q�b�g�Ƃ̊p�x�����߂�
		float radian = Vec3::dot(diff, Vec3(0.0f, 0.0f, 1.0f));

		int i = 0;
		for (int i = 0; i < m_ObjectCount; i++)
		{
			m_UnitObjects.at(i)->setDestination(destination, false);
		}
	}
}

void Unit::init(int teamID, const UnitStats* pUnitStats, ValueMap* pValueMap, UnitRenderHelper* pRenderHelper)
{
	m_ObjectCount = pUnitStats->m_ObjectCount;
	m_SpacePerObject = pUnitStats->m_SpacePerObject;
	m_TeamID = teamID;
	m_pUnitStats = pUnitStats;
	m_pValueMap = pValueMap;
	m_pRenderHelper = pRenderHelper;

	//�X�e�[�g���b�N�^�C�}�[������
	m_StateLockTimer.setMaxTime(1.0f);

	int xSize = m_ObjectCount / 1;
	int zSize = m_ObjectCount / xSize;

	Vec3 basePos(-m_SpacePerObject * (float)xSize * 0.5f, -10.0f, m_SpacePerObject * (float)zSize * 0.5f);

	for (int z = 0; z < zSize; z++)
	{
		for (int x = 0; x < xSize; x++)
		{
			//�Q�[���I�u�W�F�N�g��ǉ�
			m_GameObjects.emplace_back();
			auto& pObj = m_GameObjects.back();
			pObj = new GameObject(getUser().getGameMediator());

			//�R���|�[�l���g��ǉ�
			m_UnitObjects.emplace_back();
			auto& pUnitObject = m_UnitObjects.back();
			pUnitObject = pObj->addComponent<UnitObject>();
			pUnitObject->init(this, m_pValueMap);

			//�ړI�n��������
			auto& tr = pObj->getTransform();
			Vec3 position = Vec3(m_SpacePerObject * x, 0.0f, m_SpacePerObject * z) + basePos;
			tr.setLocalPosition(position);
			tr.setLocalScale(m_pUnitStats->m_ObjectSize);

			pUnitObject->setDestination(position);
		}
	}
}

void Unit::setPosition(const Vec3& position, float angle, int unitWidth)
{
	std::vector<Vec3> newPositions;
	calculateObjectPositions(newPositions, position, MathUtility::toRadian(angle), unitWidth);

	int i = 0;
	for (int i = 0; i < m_ObjectCount; i++)
	{
		m_UnitObjects.at(i)->setPosition(newPositions.at(i));
		m_UnitObjects.at(i)->setDestination(newPositions.at(i));
	}

	m_Angle = angle;

	//���S���W���X�V
	updateCenterPosition();
}

void Unit::setDestination(const Vec3& destination, float angle, int unitWidth, bool isMoveCommand)
{
	//�ړ��w�����������̂ōU���ڕW�̓N���A����
	m_pTargetUnit = nullptr;

	std::vector<Vec3> newPositions;
	calculateObjectPositions(newPositions, destination, MathUtility::toRadian(angle), unitWidth);

	int i = 0;
	for (int i = 0; i < m_ObjectCount; i++)
	{
		m_UnitObjects.at(i)->setDestination(newPositions.at(i), isMoveCommand);
	}

	//�X�e�[�g���b�N�J�n
	m_StateLockTimer.reset();

	m_Angle = angle;
}

float Unit::getSpacePerObject() const
{
	return m_SpacePerObject;
}

float Unit::getAngle() const
{
	return m_Angle;
}

void Unit::setTarget(Unit* pTarget)
{
	//�������̂����ɐݒ�ς݂Ȃ�ݒ肵�Ȃ�
	if (m_pTargetUnit == pTarget) return;

	//�X�e�[�g���b�N�J�n
	m_StateLockTimer.reset();

	m_pTargetUnit = pTarget;

	//�ړI�n�ݒ�
	const Vec3& destination = m_pTargetUnit->getTransform().getLocalPosition();
	for (auto pUnitObject : m_UnitObjects)
	{
		pUnitObject->setDestination(destination);
	}
}

Unit* Unit::getTarget()
{
	return m_pTargetUnit;
}

float Unit::getHealth() const
{
	//�I�u�W�F�N�g����HP���擾���č��v����
	float healthSum = 0.0f;
	for (UnitObject* pObj : m_UnitObjects)
	{
		healthSum += pObj->getHealth();
	}

	return healthSum;
}

float Unit::getShield() const
{
	//�I�u�W�F�N�g����V�[���h��HP���擾���č��v����
	float shieldSum = 0.0f;
	for (UnitObject* pObj : m_UnitObjects)
	{
		shieldSum += pObj->getShield();
	}

	return shieldSum;
}

int Unit::getObjectCount() const
{
	int count = 0;
	for (UnitObject* pObj : m_UnitObjects)
	{
		if (pObj->getHealth() > 0.0f)
			count++;
	}

	return count;
}

const UnitStats* Unit::getUnitStats() const
{
	return m_pUnitStats;
}

int Unit::getTeamID() const
{
	return m_TeamID;
}

bool Unit::isStateLocked()
{
	return m_StateLockTimer.isTime();
}

void Unit::onEnterCombat(Unit* pEnemyUnit)
{
	if (m_pTargetUnit != nullptr) return;

	setTarget(pEnemyUnit);
}

bool Unit::isInCombat() const
{
	return m_pTargetUnit != nullptr;
}

void Unit::calculateObjectPositions(std::vector<Vec3>& results, const Vec3& destination, float radian, int unitWidth)
{
	DirectX::XMMATRIX rotateMat = DirectX::XMMatrixRotationRollPitchYaw(0.0f, radian, 0.0f);

	//��̐��̓��j�b�g�̐���0���Z�ɂȂ�Ȃ��l�͈̔͂Ɏ��܂�悤�ɐ���
	m_UnitWidth = MathUtility::clamp(unitWidth, 1, m_ObjectCount);
	//�؂�グ�p�Ɋ���
	float div = (float)m_ObjectCount / (float)m_UnitWidth;
	//�؂�グ�čs�̐������߂�
	div = std::ceil(div);

	int xSize = m_UnitWidth;
	int zSize = (int)div;

	//�I�u�W�F�N�g�̐��̕�����vector�̃T�C�Y�m��
	results.reserve(m_ObjectCount);

	//�c��̌v�Z��
	int remainObjCount = m_ObjectCount;
	//�I�u�W�F�N�g���Ƃ̍��W�v�Z
	for (int z = zSize - 1; z >= 0; z--)
	{
		//���������ׂ̈ɔ���
		if (remainObjCount < m_UnitWidth)
		{
			xSize = remainObjCount;
		}

		//�x�[�X�̍��W���v�Z
		Vec3 basePos(-m_SpacePerObject * (float)(xSize - 1) * 0.5f, -10.0f, -m_SpacePerObject * (float)(zSize) * 0.5f);
		for (int x = 0; x < xSize; x++)
		{
			Vec3 newPosition = Vec3(m_SpacePerObject * x, 0.0f, m_SpacePerObject * z) + basePos;
			newPosition = newPosition.multMatrix(rotateMat) + destination;
			results.emplace_back(newPosition);
			remainObjCount--;
		}
	}
}

void Unit::updateCenterPosition()
{
	int aliveCount = 0;
	//�����Ă���I�u�W�F�N�g���J�E���g
	for (auto pUnitObject : m_UnitObjects)
	{
		if (pUnitObject->getState() != UnitObject::State::Dead)
			aliveCount++;
	}

	//���j�b�g�̒��S���W���X�V
	Vec3 sum;
	for (auto pUnitObject : m_UnitObjects)
	{
		sum += pUnitObject->getTransform().getLocalPosition();
	}
	getTransform().setLocalPosition(sum / (float)aliveCount);
}
