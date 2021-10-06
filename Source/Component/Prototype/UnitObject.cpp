#include "UnitObject.h"

#include <algorithm>

#include "Actor\Base\GameObject.h"
#include "AI\ValueMap.h"

#include "Component\Utility\Transform.h"
#include "Component\Prototype\Unit.h"
#include "Component\Box2D\CircleColliderB2.h"

#include "Device\GameDevice.h"

#include "UnitInfo\UnitStats.h"

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
	m_pValueMap->writeMap(ValueTypes::Health, Value(getTransform().getLocalPosition(), 2.0f, m_Health));

	m_AttackTimer.update();

	//�^�[�Q�b�g�����S���Ă�����^�[�Q�b�g����
	if (m_pTargetObject != nullptr)
		if (m_pTargetObject->m_State == State::Dead)
		{
			m_pTargetObject = nullptr;
			setState(State::StandBy);
		}

	//�ˌ����Ȃ�ړI�n���X�V
	if (m_State == State::Charge &&
		m_pTargetObject != nullptr)
		m_Destination = m_pTargetObject->getTransform().getLocalPosition();

	//�ҋ@���łȂ��A���퓬���łȂ��Ȃ�ړ�����
	if (m_State != State::StandBy &&
		m_State != State::Combat &&
		m_State != State::CombatStandBy)
		move();
	else
		m_pCollider->setVelocity(0.0f, 0.0f);

	if (m_State == State::Combat &&
		m_pTargetObject != nullptr)
		attack();
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
	m_pTrigger->setRadius(1.5f);

	//�U���Ԋu��ݒ�
	m_AttackTimer.setMaxTime(pUnitStats->m_AttackInterval);

	m_pValueMap = pValueMap;
}

void UnitObject::setDestination(const Vec3& destination, bool moveCommand)
{
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
	m_pCollider->setVelocity(moveDir.x * 10.0f, moveDir.z * 10.0f);
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
		setState(State::Combat);
		//���j�b�g�ɒʒm
		m_pUnit->onEnterCombat(m_pTargetObject->m_pUnit);
	}

	trySetTargetObject(pUnitObject, State::Combat);
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
	//�퓬�ҋ@���Ȃ牽���s��Ȃ�
	if (m_State == State::CombatStandBy) return;

	trySetTargetObject(pUnitObject, State::Charge);

	trySetPredecessor(pUnitObject);
}

void UnitObject::onTriggerExit(UnitObject* pUnitObject)
{
}

int UnitObject::getTeamID() const
{
	return m_pUnit->getTeamID();
}

void UnitObject::takeDamage(float damage)
{
	float armor = m_pUnit->getUnitStats()->m_Armor;
	//�_���[�W�J�b�g���v�Z
	float damageCut = armor / 100.0f;
	//�_���[�W���󂯂�
	m_Health -= damage * std::fmaxf((1.0f - damageCut), 0.0f);
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

	//���p������
	tryTakeOverTarget();
}

void UnitObject::attack()
{
	if (!m_AttackTimer.isTime()) return;

	const UnitStats* pUnitStats = m_pUnit->getUnitStats();
	const UnitStats* pTargetUnitStats = m_pTargetObject->m_pUnit->getUnitStats();
	Random& random = GameDevice::getRandom();

	//�h��m���Ɩ����m���̍�
	int diff = 35 + pTargetUnitStats->m_MeleeAttack - pUnitStats->m_MeleeDefence;
	//�������邩����
	bool isHit = random.getRandom(0, 100) <= diff;

	//�������Ă�����_���[�W
	if (isHit)
		m_pTargetObject->takeDamage(pUnitStats->m_MeleeDamage);

	m_AttackTimer.reset();
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

	//�X�e�[�g�ύX
	setState(nextState);

	if (nextState == State::Combat)
	{
		//���j�b�g�ɒʒm
		m_pUnit->onEnterCombat(m_pTargetObject->m_pUnit);
	}
}

void UnitObject::trySetPredecessor(UnitObject* pPredecessor)
{
	//�O�C���ݒ肳��Ă���Ȃ���s���Ȃ�
	if (m_pPredecessor != nullptr) return;

	const State& predState = pPredecessor->getState();

	//�������퓬���܂��͓ˌ����Ȃ���s���Ȃ�
	if (m_State == State::Combat || m_State == State::Charge) return;

	//�O�C��₪�퓬���A�܂��͐퓬�ҋ@���őO�C�҂�����Ȃ���s����
	if (predState == State::Combat ||
		(predState == State::CombatStandBy && pPredecessor->m_pPredecessor != nullptr))
	{
		//�O�C��ݒ�
		m_pPredecessor = pPredecessor;
		//�O�C�̌�C�������ɐݒ�
		m_pPredecessor->m_pSuccessor = this;

		//�퓬�ҋ@��ԂɈڍs
		setState(State::CombatStandBy);
	}
}

void UnitObject::tryTakeOverTarget()
{
	//��C�����Ȃ��Ȃ�return
	if (m_pSuccessor == nullptr || m_pTargetObject == nullptr) return;

	//��C�Ɉ��p��
	m_pSuccessor->trySetTargetObject(m_pTargetObject, State::Charge);

	//�O�C(����)��o�^����
	m_pSuccessor->m_pPredecessor = nullptr;
}

void UnitObject::setState(const State& newState)
{
	if (m_State == State::Dead) return;
	if (m_State == newState) return;

	//�ړ��ȊO�̃X�e�[�g&�X�e�[�g�����b�N����Ă���Ȃ���s���Ȃ��B���S�͒ʂ��B
	//if (newState != State::Move && newState != State::Dead && m_pUnit->isStateLocked()) return;
	//if (newState != State::Move && newState != State::Dead) return;

	//�퓬�ȊO�̃X�e�[�g�Ɉڍs������O�C�ƌ�C����������
	//if (newState != State::Combat)
	//{
	//	m_pSuccessor = nullptr;
	//	m_pPredecessor = nullptr;
	//}

	m_State = newState;
}
