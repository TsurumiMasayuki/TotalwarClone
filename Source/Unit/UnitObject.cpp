#include "UnitObject.h"

#include <algorithm>

#include "Actor\Base\GameObject.h"
#include "AI\ValueMap.h"

#include "Component\Utility\Transform.h"
#include "Component\Box2D\CircleColliderB2.h"

#include "Device\GameDevice.h"

#include "Unit\Unit.h"
#include "Unit\UnitStats.h"
#include "Unit\Attack\Attack.h"
#include "Utility\JsonFileManager.h"

#include "Math\MathUtility.h"
#include "Math\Easing.h"

#include "GameState.h"

#include "Effect\TestEffect_Beam.h"

void UnitObject::onStart()
{
	m_ShieldRegenTimer.setMaxTime(1.0f);
}

void UnitObject::onUpdate()
{
	//�����t�F�[�Y���Ȃ牽���s��Ȃ�
	if (Game::g_GameState == Game::GameState::PreparePhase)
		return;

	//�X�e�[�g�J��
	stateTransition();

	//����������ł����牽���s��Ȃ�
	if (m_State == State::Dead)
		return;

	//�w���X�̒l����������
	m_pValueMap->writeMap(UnitStatsValues::Health, Value(getTransform().getLocalPosition(), 25.0f, m_Health));

	updateShield();

	//�^�[�Q�b�g�����S���Ă�����^�[�Q�b�g����
	if (m_pTargetObject != nullptr)
	{
		if (m_pTargetObject->m_State == State::Dead)
		{
			m_pTargetObject = nullptr;

			//�U���𖳌���
			for (auto pAttacks : m_MainAttacks)
			{
				pAttacks->setTarget(nullptr);
				pAttacks->setActive(false);
			}
		}
	}

	switch (m_State)
	{
	case UnitObject::State::StandBy:
		m_pCollider->setVelocity(0.0f, 0.0f);
		break;
	case UnitObject::State::Move:
		move();
		rotate();
		break;
	case UnitObject::State::Charge:
		if (m_pTargetObject != nullptr)
		{
			//���g�̍��W
			const Vec3& myPos = getTransform().getLocalPosition();
			//�^�[�Q�b�g�̍��W
			const Vec3& targetPos = m_pTargetObject->getTransform().getLocalPosition();

			//���g�̍��W�ƃ^�[�Q�b�g�̍��W����U����������ł��߂����W���Z�o���ĖړI�n�ɂ���
			float range = m_pLongestMainAttack->getAttackRange();
			Vec3 diff = targetPos - myPos;
			if (diff.sqrLength() < range * range)
			{
				m_Destination = myPos;
			}
			else
			{
				m_Destination = targetPos + diff.normalized() * range;
			}
		}
		move();
		rotate();
		break;
	case UnitObject::State::Attack:
		//���C���U�����X�V
		for (auto pAttack : m_MainAttacks)
		{
			pAttack->update();
		}
		//�T�u�U�����X�V
		for (auto pAttack : m_SubAttacks)
		{
			pAttack->update();
		}
		m_pCollider->setVelocity(0.0f, 0.0f);

		rotate();

		break;
	}
}

void UnitObject::onDestroy()
{
	//���C���U���̃C���X�^���X���폜
	for (auto pAttack : m_MainAttacks)
	{
		delete pAttack;
	}

	//�T�u�U���̃C���X�^���X���폜
	for (auto pAttack : m_SubAttacks)
	{
		delete pAttack;
	}
}

void UnitObject::init(Unit* pUnit, ValueMap* pValueMap, EffectRenderHelper* pEffectRenderHelper)
{
	m_pUnit = pUnit;
	setState(State::StandBy);

	const UnitStats* pUnitStats = m_pUnit->getUnitStats();
	m_Health = pUnitStats->m_MaxHealthPerObject;
	m_Shield = pUnitStats->m_MaxShieldPerObject;
	m_pValueMap = pValueMap;

	for (auto& attacks : m_pUnit->getUnitStats()->getMainAttacks())
	{
		auto pEffectObj = new GameObject(getUser().getGameMediator());

		//�G�t�F�N�g����
		m_AttackEffects.emplace_back(
			pEffectObj->addComponent<TestEffect_Beam>()
		);

		auto pAttackEffect = m_AttackEffects.back();
		pAttackEffect->init(pEffectRenderHelper);

		//�U���N���X����
		m_MainAttacks.emplace_back(
			new Attack(this, 
				&JsonFileManager<AttackStats>::getInstance().get(attacks.m_AttackName),
				pAttackEffect)
		);
	}

	//�˒��������������Ƀ\�[�g
	std::sort(m_MainAttacks.begin(), m_MainAttacks.end(),
		[](Attack* a, Attack* b)
		{
			return a->getAttackRange() > b->getAttackRange();
		}
	);

	//�ł��˒������������U����o�^
	m_pLongestMainAttack = m_MainAttacks.at(0);

	//�ʏ�R���C�_�[
	m_pCollider = getUser().addComponent<CircleColliderB2>();
	m_pCollider->setTrigger(false);
	m_pCollider->setRadius(1.0f);
	m_pCollider->setBodyType(b2BodyType::b2_dynamicBody);
	m_pCollider->setGroupIndex(0);

	//�g���K�[�p�R���C�_�[
	m_pTrigger = getUser().addComponent<CircleColliderB2>();
	m_pTrigger->setTrigger(true);
	//�˒������Ɠ����T�C�Y�ɂ��邽�߂ɃX�P�[���Ŋ���
	float scaleX = getTransform().getLocalScale().x;
	m_pTrigger->setRadius(m_pLongestMainAttack->getAttackRange() / scaleX);
	m_pTrigger->setGroupIndex(-1);
}

void UnitObject::setDestination(const Vec3& destination, bool isMoveCommand)
{
	//�^�[�Q�b�g�����Ĉړ����߂łȂ��Ȃ�return
	if (m_pTargetObject != nullptr &&
		!isMoveCommand &&
		m_State == State::Attack) return;

	m_Destination = destination;
	m_Destination.y = 0.0f;

	if (isMoveCommand)
	{
		m_pTargetObject = nullptr;
		setState(State::Move);
	}
}

void UnitObject::move()
{
	Vec3 diff = m_Destination - getTransform().getLocalPosition();
	diff.y = 0.0f;

	//���X�Ɍ���
	float ratio = std::fminf(1.0f, diff.length() / m_pUnit->getUnitStats()->m_Speed);
	ratio = Easing::easeOutExpo(ratio) * m_pUnit->getUnitStats()->m_Speed;
	m_pCollider->setVelocity(m_Direction.x * ratio, m_Direction.z * ratio);
}

void UnitObject::rotate()
{
	//��]���������߂�
	Vec3 diff = m_Destination - getTransform().getLocalPosition();
	if (m_pTargetObject != nullptr)
		diff = m_pTargetObject->getTransform().getLocalPosition() - getTransform().getLocalPosition();
	diff.y = 0.0f;

	//�������p�x�ɕϊ�
	float desiredAngle = MathUtility::toDegree(std::atan2f(diff.x, diff.z));

	//�ړI�n�Ɍ����ĉ�]
	float curAngle = getTransform().getLocalAngles().y;

	//�g�p����p�x
	float angle = MathUtility::lerp(curAngle, desiredAngle, 1.0f);
	float radian = MathUtility::toRadian(angle);

	//�ړ��������X�V
	m_Direction = Vec3(std::sinf(radian), 0.0f, std::cosf(radian));

	getTransform().setLocalAngles(Vec3(0.0f,
		angle,
		0.0f)
	);
}

void UnitObject::resetCollider()
{
	m_pCollider->init();
	m_pTrigger->init();
}

void UnitObject::onCollisionEnter(UnitObject* pUnitObject)
{
}

void UnitObject::onCollisionStay(UnitObject* pUnitObject)
{
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
	//�V�[���h������������HP�Ń_���[�W���󂯂�B�����łȂ���΃V�[���h�Ŏ󂯂�B
	if (m_Shield <= 0.0f)
	{
		m_Health -= damage;
		m_Health = std::fmaxf(0.0f, m_Health);
	}
	else
	{
		m_Shield -= damage;
		m_Shield = std::fmaxf(0.0f, m_Shield);
	}

	//�V�[���h�񕜂̃^�C�}�[�����Z�b�g
	m_ShieldRegenTimer.reset();

	//���S����
	if (m_Health <= 0.0f)
		setState(State::Dead);
}

float UnitObject::getHealth() const
{
	return m_Health;
}

float UnitObject::getShield() const
{
	return m_Shield;
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
	if (m_Health <= 0.0f)
	{
		setState(State::Dead);
		getUser().setActive(false);
		return;
	}

	const Vec3& myPos = getTransform().getLocalPosition();

	//�U���p�X�e�[�g�J��
	if (m_pTargetObject != nullptr)
	{
		const Vec3& targetPos = m_pTargetObject->getTransform().getLocalPosition();

		float sqrDistance = myPos.sqrDistance(targetPos);
		float range = m_MainAttacks.at(0)->getAttackRange();
		if (sqrDistance < range * range)
		{
			setState(State::Attack);
		}
		else
		{
			//setState(State::Charge);
		}
	}
	//�ړ��p�X�e�[�g�J��
	else
	{
		float distance = myPos.distance(m_Destination);
		if (distance > 0.15f)
		{
			setState(State::Move);
		}
		else
		{
			setState(State::StandBy);
		}
	}
}

void UnitObject::trySetTargetObject(UnitObject* pTargetObject, const State& nextState)
{
	//�^�[�Q�b�g���ݒ�ς݂Ȃ���s���Ȃ�
	if (m_pTargetObject != nullptr) return;

	//�����`�[���Ȃ���s���Ȃ�
	if (getTeamID() == pTargetObject->getTeamID()) return;

	//�X�e�[�g���b�N���Ȃ���s���Ȃ�
	if (m_pUnit->isStateLocked()) return;

	//�^�[�Q�b�g�ݒ�
	m_pTargetObject = pTargetObject;
	for (auto pAttack : m_MainAttacks)
	{
		//�U����L����
		pAttack->setActive(true);
		pAttack->setTarget(m_pTargetObject);
	}
}

void UnitObject::setState(const State& newState)
{
	if (m_State == State::Dead) return;
	if (m_State == newState) return;

	m_State = newState;

	if (m_State == State::Attack)
	{
		//���j�b�g�ɒʒm
		m_pUnit->onEnterCombat(m_pTargetObject->m_pUnit);
	}
}

void UnitObject::updateShield()
{
	m_ShieldRegenTimer.update();

	//�V�[���h�񕜊J�n
	if (m_ShieldRegenTimer.isTime())
	{
		//�V�[���h�̍ő�l�𒴂��Ȃ��悤�ɉ�
		m_Shield = std::fminf(m_pUnit->getUnitStats()->m_MaxShieldPerObject,
			m_Shield + 10.0f * GameDevice::getGameTime().getDeltaTime());
	}
}
