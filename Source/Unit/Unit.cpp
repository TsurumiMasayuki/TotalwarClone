#include "Unit.h"

#include <algorithm>

#include "Actor\Base\GameObject.h"
#include "AI\ValueMap.h"

#include "Component\Physics\SphereColliderBt.h"
#include "Math\MathUtility.h"

#include "Component\Box2D\CircleColliderB2.h"
#include "Unit\UnitObject.h"

//TODO:��]��񎟌����W�p�ɕς���

void Unit::onStart()
{
}

void Unit::onUpdate()
{
	m_StateLockTimer.update();

	//���j�b�g�̒��S���W���X�V
	int aliveCount = 0;
	Vec3 sum;
	for (auto pUnitObject : m_UnitObjects)
	{
		//����ł���I�u�W�F�N�g�͊܂߂Ȃ�
		if (pUnitObject->getState() == UnitObject::State::Dead) continue;

		aliveCount++;
		sum += pUnitObject->getTransform().getLocalPosition();
	}

	//0���Z�h�~
	if (aliveCount != 0)
		getTransform().setLocalPosition(sum / (float)aliveCount);


	//�U���w��������Ȃ�^�[�Q�b�g�Ɍ����Ĉړ�
	if (m_pTargetUnit != nullptr)
	{
		const Vec3& pos = getTransform().getLocalPosition();
		const Vec3& destination = m_pTargetUnit->getTransform().getLocalPosition();
		//�^�[�Q�b�g�Ƃ̊p�x�����߂�
		float radian = std::atan2f(destination.y - pos.y, destination.x - pos.x) + MathUtility::toRadian(90.0f);

		std::vector<Vec3> newPositions;
		this->calculateObjectPositions(newPositions, destination, radian);

		int i = 0;
		for (int i = 0; i < m_UnitCount; i++)
		{
			if (m_UnitObjects.at(i)->getState() == UnitObject::State::Move)
				m_UnitObjects.at(i)->setDestination(newPositions.at(i), false);
		}
	}

	//InstancedRenderer�ɏ��𑗂�
	std::vector<UnitInstanceInfo> instanceInfo;
	for (int i = 0; i < (int)m_UnitObjects.size(); i++)
	{
		instanceInfo.emplace_back();
		auto& instance = instanceInfo.back();
		auto world = m_UnitObjects[i]->getTransform().getWorldMatrix();
		auto instanceMat = DirectX::XMMatrixTranspose(world);

		//���S��ԂȂ��\��
		if (m_UnitObjects[i]->getState() == UnitObject::State::Dead)
			instanceMat = DirectX::XMMatrixSet(
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f);

		DirectX::XMStoreFloat4x4(&instance.instanceMat, instanceMat);
		DirectX::XMStoreFloat4(&instance.instanceColor, DirectX::Colors::White);
	}

	m_pInstancedRenderer->setInstanceInfo(instanceInfo);
}

void Unit::init(int unitCount, float spacePerObject, int teamID, UnitStats* pUnitStats, ValueMap* pValueMap)
{
	m_UnitCount = unitCount;
	m_SpacePerObject = spacePerObject;
	m_TeamID = teamID;
	m_pUnitStats = pUnitStats;
	m_pValueMap = pValueMap;

	//�X�e�[�g���b�N�^�C�}�[������
	m_StateLockTimer.setMaxTime(1.0f);

	m_pInstancedRenderer = getUser().getComponent<InstancedRenderer<UnitInstanceInfo>>();

	int xSize = m_UnitCount / 5;
	int zSize = m_UnitCount / xSize;

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
			tr.setLocalScale(Vec3(0.5f, 0.5f, 1.0f));

			pUnitObject->setDestination(position);
		}
	}
}

void Unit::setPosition(const Vec3& position, float angle)
{
	std::vector<Vec3> newPositions;
	this->calculateObjectPositions(newPositions, position, MathUtility::toRadian(angle));

	int i = 0;
	for (int i = 0; i < m_UnitCount; i++)
	{
		{
			auto& tr = m_UnitObjects.at(i)->getTransform();
			tr.setLocalPosition(newPositions.at(i));
			m_UnitObjects.at(i)->setDestination(newPositions.at(i));
		}
	}

	m_Angle = angle;
}

void Unit::setDestination(const Vec3& destination, float angle)
{
	//�ړ��w�����������̂ōU���ڕW�̓N���A����
	m_pTargetUnit = nullptr;

	std::vector<Vec3> newPositions;
	this->calculateObjectPositions(newPositions, destination, MathUtility::toRadian(angle));

	int i = 0;
	for (int i = 0; i < m_UnitCount; i++)
	{
		{
			m_UnitObjects.at(i)->setDestination(newPositions.at(i));
		}
	}

	//�X�e�[�g���b�N�J�n
	m_StateLockTimer.reset();

	m_Angle = angle;
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

void Unit::calculateObjectPositions(std::vector<Vec3>& results, const Vec3& destination, float radian)
{
	DirectX::XMMATRIX rotateMat = DirectX::XMMatrixRotationRollPitchYaw(0.0f, radian, 0.0f);

	int xSize = m_UnitCount / 5;
	int zSize = m_UnitCount / xSize;

	results.reserve(m_UnitCount);

	Vec3 basePos(-m_SpacePerObject * (float)(xSize - 1) * 0.5f, -10.0f, m_SpacePerObject * (float)(zSize - 1) * 0.5f);

	for (int z = 0; z < zSize; z++)
	{
		for (int x = 0; x < xSize; x++)
		{
			Vec3 newPosition = Vec3(m_SpacePerObject * x, 0.0f, m_SpacePerObject * z) + basePos;
			newPosition = newPosition.multMatrix(rotateMat) + destination;
			results.emplace_back(newPosition);
		}
	}
}
