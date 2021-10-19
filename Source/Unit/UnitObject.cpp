#include "UnitObject.h"

#include <algorithm>

#include "Actor\Base\GameObject.h"
#include "AI\ValueMap.h"

#include "Component\Utility\Transform.h"
#include "Component\Box2D\CircleColliderB2.h"

#include "Device\GameDevice.h"

#include "Unit\Attack\Attack.h"
#include "Unit\Unit.h"
#include "UnitInfo\UnitStats.h"
#include "Math\MathUtility.h"

void UnitObject::onStart()
{
}

void UnitObject::onUpdate()
{
	//�X�e�[�g�J��
	stateTransition();

	//����������ł����牽���s��Ȃ�
	if (m_State == State::Dead)
		return;

	//�w���X�̒l����������
	m_pValueMap->writeMap(UnitStatsValues::Health, Value(getTransform().getLocalPosition(), 25.0f, m_Health));

	rotate();

	//�^�[�Q�b�g�����S���Ă�����^�[�Q�b�g����
	if (m_pTargetObject != nullptr)
		if (m_pTargetObject->m_State == State::Dead)
		{
			m_pTargetObject = nullptr;
			setState(State::Move);
		}

	//�ˌ����Ȃ�ړI�n���X�V
	if (m_State == State::Charge &&
		m_pTargetObject != nullptr)
		m_Destination = m_pTargetObject->getTransform().getLocalPosition();

	//�ҋ@���łȂ��A���퓬���łȂ��Ȃ�ړ�����
	if (m_State != State::StandBy &&
		m_State != State::Attack)
		move();
	else
		m_pCollider->setVelocity(0.0f, 0.0f);

	if (m_State == State::Attack &&
		m_pTargetObject != nullptr)
		m_pAttack->update();
}

void UnitObject::onDestroy()
{
	delete m_pAttack;
}

void UnitObject::init(Unit* pUnit, ValueMap* pValueMap)
{
	m_pUnit = pUnit;
	setState(State::StandBy);

	const UnitStats* pUnitStats = m_pUnit->getUnitStats();
	m_Health = pUnitStats->m_MaxHealthPerObject;

	//�ʏ�R���C�_�[
	m_pCollider = getUser().addComponent<CircleColliderB2>();
	m_pCollider->setTrigger(false);
	m_pCollider->setRadius(1.0f);

	//�g���K�[�p�R���C�_�[
	m_pTrigger = getUser().addComponent<CircleColliderB2>();
	m_pTrigger->setTrigger(true);
	m_pTrigger->setRadius(15.0f);

	m_pValueMap = pValueMap;

	//�U���N���X����
	m_pAttack = new Attack(
		10.0f,
		0.5f,
		10.0f,
		10.0f,
		true,
		100,
		false,
		this
	);
}

void UnitObject::setDestination(const Vec3& destination, bool moveCommand)
{
	//�^�[�Q�b�g�����Ĉړ����߂łȂ��Ȃ�return
	if (m_pTargetObject != nullptr && !moveCommand) return;

	m_Destination = destination;

	if (moveCommand)
	{
		m_pTargetObject = nullptr;
		setState(State::Move);
	}
}

void UnitObject::move()
{
	Vec3 diff = m_Destination - getTransform().getLocalPosition();
	diff.y = 0.0f;

	//�ړI�n�ɋ߂��Ȃ������~
	if (diff.length() < 0.25f &&
		m_State == State::Move)
	{
		m_State = State::StandBy;
		return;
	}

	//�ړ�
	Vec3 moveDir = diff.normalized();
	m_pCollider->setVelocity(moveDir.x * m_pUnit->getUnitStats()->m_Speed, moveDir.z * m_pUnit->getUnitStats()->m_Speed);
}

void UnitObject::rotate()
{
	Vec3 diff = m_Destination - getTransform().getLocalPosition();
	diff.y = 0.0f;

	//�ړ�
	Vec3 rotateDir = diff.normalized();

	//�ړI�n�Ɍ����ĉ�]
	m_DesiredAngle = MathUtility::toDegree(std::atan2f(rotateDir.z, rotateDir.x)) + 90.0f;
	float curAngle = getTransform().getLocalAngles().y;
	getTransform().setLocalAngles(Vec3(0.0f, MathUtility::lerp(curAngle, m_DesiredAngle, GameDevice::getGameTime().getDeltaTime() * 2.0f)));
}

void UnitObject::onCollisionEnter(UnitObject* pUnitObject)
{
}

void UnitObject::onCollisionStay(UnitObject* pUnitObject)
{
	//����������ł����牽���s��Ȃ�
	if (m_State == State::Dead) return;

	if (pUnitObject == m_pTargetObject)
	{
		setState(State::Attack);
		//���j�b�g�ɒʒm
		m_pUnit->onEnterCombat(m_pTargetObject->m_pUnit);
	}

	trySetTargetObject(pUnitObject, State::Attack);
}

void UnitObject::onCollisionExit(UnitObject* pUnitObject)
{
}

void UnitObject::onTriggerEnter(UnitObject* pUnitObject)
{
}

void UnitObject::onTriggerStay(UnitObject* pUnitObject)
{
	//����������ł����牽���s��Ȃ�
	if (m_State == State::Dead) return;

	trySetTargetObject(pUnitObject, State::Charge);
}

void UnitObject::onTriggerExit(UnitObject* pUnitObject)
{
	m_pTargetObject = nullptr;
}

int UnitObject::getTeamID() const
{
	return m_pUnit->getTeamID();
}

void UnitObject::takeDamage(float damage)
{
	//�_���[�W���󂯂�
	m_Health -= damage;
}

float UnitObject::getHealth() const
{
	return m_Health;
}

Unit* UnitObject::getUnit()
{
	return m_pUnit;
}

const UnitObject::State& UnitObject::getState() const
{
	return m_State;
}

void UnitObject::stateTransition()
{
	//���S����
	if (m_Health > 0.0f) return;

	m_Health = 0.0f;
	setState(State::Dead);

	getUser().setActive(false);
}

void UnitObject::trySetTargetObject(UnitObject* pTargetObject, const State& nextState)
{
	//�^�[�Q�b�g���ݒ�ς݂Ȃ���s���Ȃ�
	if (m_pTargetObject != nullptr) 
		return;

	//�����`�[���Ȃ珈�����s��Ȃ�
	if (getTeamID() == pTargetObject->getTeamID()) return;

	//�^�[�Q�b�g�ݒ�
	m_pTargetObject = pTargetObject;
	m_pAttack->setTarget(m_pTargetObject);

	//�˒�����
	float range = m_pAttack->getAttackRange();
	//���g�̍��W
	const Vec3& myPos = getTransform().getLocalPosition();
	//�^�[�Q�b�g�̍��W
	const Vec3& targetPos = pTargetObject->getTransform().getLocalPosition();

	//���g�̍��W�ƃ^�[�Q�b�g�̍��W����U����������ł��߂����W���Z�o���ĖړI�n�ɂ���
	Vec3 diff = targetPos - myPos;
	m_Destination = myPos + diff.normalized() * range;

	//�X�e�[�g�ύX
	setState(nextState);

	if (nextState == State::Attack)
	{
		//���j�b�g�ɒʒm
		m_pUnit->onEnterCombat(m_pTargetObject->m_pUnit);
	}
}

void UnitObject::setState(const State& newState)
{
	if (m_State == State::Dead) return;
	if (m_State == newState) return;

	//�ړ��ȊO�̃X�e�[�g&�X�e�[�g�����b�N����Ă���Ȃ���s���Ȃ��B���S�͒ʂ��B
	//if (newState != State::Move && newState != State::Dead && m_pUnit->isStateLocked()) return;
	//if (newState != State::Move && newState != State::Dead) return;

	//�퓬�ȊO�̃X�e�[�g�Ɉڍs������O�C�ƌ�C����������
	//if (newState != State::Attack)
	//{
	//	m_pSuccessor = nullptr;
	//	m_pPredecessor = nullptr;
	//}

	m_State = newState;
}
