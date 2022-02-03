#include "Unit.h"

#include <algorithm>

#include "Actor\Base\GameObject.h"
#include "Component\Physics\SphereColliderBt.h"
#include "Math\MathUtility.h"

#include "AI\ValueMap.h"
#include "Component\Box2D\CircleColliderB2.h"
#include "Unit\UnitObject.h"
#include "Unit\UnitRenderHelper.h"
#include "Utility\JsonFileManager.h"
#include "Player\IPlayer.h"

void Unit::onStart()
{
}

void Unit::onUpdate()
{
	//�����Ă���I�u�W�F�N�g�����Ȃ��Ȃ�return
	if (getObjectCount() == 0)
		return;

	//�^�[�Q�b�g��������
	if (m_pTargetUnit != nullptr)
	{
		float sqrDistance = m_pTargetUnit->getTransform().getLocalPosition().sqrDistance(getTransform().getLocalPosition());

		//�^�[�Q�b�g���S�ł��Ă���or�U���͈͂���o���Ȃ����
		if (m_pTargetUnit->getObjectCount() == 0 ||
			sqrDistance < m_MainAttackRange * m_MainAttackRange)
			m_pTargetUnit = nullptr;
	}

	//�C���X�^���V���O�p���𑗂�
	std::vector<DirectX::XMMATRIX> objMatrices;
	for (auto pUnitObject : m_UnitObjects)
	{
		if (!pUnitObject->isDead())
			objMatrices.emplace_back(pUnitObject->getTransform().getWorldMatrix());
	}
	m_pUnitRenderHelper->appendInstanceInfo(objMatrices);

	updateCenterPosition();
}

void Unit::onDestroy()
{
	for (auto pUnitObject : m_UnitObjects)
	{
		pUnitObject->getUser().destroy();
	}

	m_pOwnerPlayer->removeUnit(this);
}

void Unit::onEnable()
{
	for (auto pUnitObject : m_UnitObjects)
	{
		pUnitObject->getUser().setActive(true);
	}
}

void Unit::onDisable()
{
	for (auto pUnitObject : m_UnitObjects)
	{
		pUnitObject->getUser().setActive(false);
	}

	m_pOwnerPlayer->removeUnit(this);
}

void Unit::init(IPlayer* pPlayer, const UnitStats* pUnitStats, ValueMap* pValueMap, UnitRenderHelper* pUnitRenderHelper, EffectRenderHelper* pEffectRenderHelper, bool isControlledByHuman)
{
	int objectCount = pUnitStats->m_ObjectCount;
	m_TeamID = pPlayer->getTeamID();
	m_pUnitStats = pUnitStats;
	m_pValueMap = pValueMap;
	m_pUnitRenderHelper = pUnitRenderHelper;
	m_pOwnerPlayer = pPlayer;
	m_IsControlledByHuman = isControlledByHuman;

	int xSize = objectCount;
	int zSize = objectCount / xSize;

	Vec3 basePos(-pUnitStats->m_SpacePerObject * (float)xSize * 0.5f, -10.0f, pUnitStats->m_SpacePerObject * (float)zSize * 0.5f);

	m_ObjectPlacement.setSpacePerObject(pUnitStats->m_SpacePerObject);
	m_ObjectPlacement.setBasePos(basePos);
	m_ObjectPlacement.setWidth(xSize);
	m_ObjectPlacement.setAngle(0.0f);

	for (int z = 0; z < objectCount; z++)
	{
		//�Q�[���I�u�W�F�N�g��ǉ�
		m_GameObjects.emplace_back();
		auto pObj = m_GameObjects.back();
		pObj = new GameObject(getUser().getGameMediator());
		auto& transform = pObj->getTransform();
		transform.setLocalScale(m_pUnitStats->m_ModelSize);

		//�R���|�[�l���g��ǉ�
		m_UnitObjects.emplace_back();
		auto& pUnitObject = m_UnitObjects.back();
		pUnitObject = pObj->addComponent<UnitObject>();
		pUnitObject->init(this, m_pValueMap, pEffectRenderHelper);

		//�I�u�W�F�N�g�z�u�ɓo�^
		m_ObjectPlacement.addObject(&transform);
	}
	
	m_MainAttackRange = JsonFileManager<AttackStats>::getInstance().get(m_pUnitStats->getMainAttacks().at(0).m_AttackName).m_AttackRange;
}

void Unit::setPosition(const Vec3& position, float angle, int unitWidth)
{
	//���j�b�g�I�u�W�F�N�g�̍��W�ݒ�
	m_ObjectPlacement.setBasePos(position);
	m_ObjectPlacement.setAngle(angle);
	m_ObjectPlacement.setWidth(unitWidth);
	//���W�v�Z���ēK�p
	m_ObjectPlacement.applyObjectPositions();

	//�R���C�_�[�����Z�b�g
	for (auto pUnitObject : m_UnitObjects)
	{
		pUnitObject->setDestination(pUnitObject->getTransform().getLocalPosition());
		pUnitObject->getTransform().setLocalAngles(Vec3(0.0f, angle, 0.0f));
		pUnitObject->resetCollider();
	}

	//���S���W���X�V
	updateCenterPosition();
}

void Unit::setDestination(const Vec3& destination, float angle, int unitWidth, bool isMoveCommand)
{
	m_ObjectPlacement.setBasePos(destination);
	m_ObjectPlacement.setWidth(unitWidth);
	m_ObjectPlacement.setAngle(angle);

	std::vector<Vec3> newPositions;
	m_ObjectPlacement.calculateObjectPositions(newPositions);

	for (int i = 0; i < (int)newPositions.size(); i++)
	{
		m_UnitObjects.at(i)->setDestination(newPositions.at(i), isMoveCommand);
	}
}

float Unit::getSpacePerObject() const
{
	return m_ObjectPlacement.getSpacePerObject();
}

float Unit::getAngle() const
{
	return m_ObjectPlacement.getAngle();
}

int Unit::getWidth() const
{
	return m_ObjectPlacement.getWidth();
}

void Unit::setTarget(Unit* pTarget, bool isPlayerCommand)
{
	//�������̂����ɐݒ�ς݂Ȃ�ݒ肵�Ȃ�
	if (m_pTargetUnit == pTarget &&
		!isPlayerCommand) return;

	m_pTargetUnit = pTarget;

	//AI�R���g���[���łȂ���ΖړI�n���ݒ�
	if (!isPlayerCommand) return;

	const Vec3& myPos = getTransform().getLocalPosition();
	const Vec3& targetPos = m_pTargetUnit->getTransform().getLocalPosition();
	Vec3 diff = targetPos - myPos;

	//�U���͈͂̃M���M����ړI�n�ɐݒ�
	Vec3 destination = targetPos + -diff.normalized() * (m_MainAttackRange - 5.0f);
	float radian = atan2f(diff.x, diff.z);

	//�ړI�n�ݒ�
	setDestination(destination, MathUtility::toDegree(radian), getWidth());
}

Unit* Unit::getTarget()
{
	return m_pTargetUnit;
}

float Unit::getHealth() const
{
	//�I�u�W�F�N�g����HP���擾���č��v����
	float healthSum = 0.0f;
	for (UnitObject* pUnitObject : m_UnitObjects)
	{
		healthSum += pUnitObject->getHealth();
	}

	return healthSum;
}

float Unit::getShield() const
{
	//�I�u�W�F�N�g����V�[���h��HP���擾���č��v����
	float shieldSum = 0.0f;
	for (UnitObject* pUnitObject : m_UnitObjects)
	{
		shieldSum += pUnitObject->getShield();
	}

	return shieldSum;
}

int Unit::getObjectCount() const
{
	int count = 0;
	for (UnitObject* pUnitObject : m_UnitObjects)
	{
		if (pUnitObject->getHealth() > 0.0f)
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

void Unit::onEnterCombat(Unit* pEnemyUnit)
{
	if (m_pTargetUnit != nullptr) return;

	setTarget(pEnemyUnit);
}

bool Unit::isInCombat() const
{
	return m_pTargetUnit != nullptr;
}

std::vector<UnitObject*>& Unit::getUnitObjects()
{
	return m_UnitObjects;
}

float Unit::getAttackRange() const
{
	return m_MainAttackRange;
}

void Unit::updateCenterPosition()
{
	int aliveCount = 0;
	//�����Ă���I�u�W�F�N�g���J�E���g
	for (auto pUnitObject : m_UnitObjects)
	{
		if (!pUnitObject->isDead())
			aliveCount++;
	}

	//0���Z�h�~
	if (aliveCount == 0) return;

	//���j�b�g�̒��S���W���X�V
	Vec3 sum;
	for (auto pUnitObject : m_UnitObjects)
	{
		if (!pUnitObject->isDead())
			sum += pUnitObject->getTransform().getLocalPosition();
	}
	getTransform().setLocalPosition(sum / (float)aliveCount);
}
