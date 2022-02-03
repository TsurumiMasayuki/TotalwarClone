#include "UnitObject.h"
#include <algorithm>

#include "Actor\Base\GameObject.h"

#include "Component\Utility\Transform.h"

#include "Device\GameDevice.h"

#include "AI\ValueMap.h"

#include "Component\Box2D\CircleColliderB2.h"

#include "Unit\Unit.h"
#include "Unit\UnitStats.h"
#include "Unit\Attack\Attack.h"
#include "Utility\JsonFileManager.h"

#include "Math\MathUtility.h"
#include "Math\Easing.h"

#include "GameState.h"

#include "Effect\TestEffect_Beam.h"
#include "Effect\CubeTrailEffect.h"

void UnitObject::onStart()
{
	m_ShieldRegenTimer.setMaxTime(1.0f);
}

void UnitObject::onUpdate()
{
	//�����t�F�[�Y���Ȃ牽���s��Ȃ�
	if (Game::g_GameState == Game::GameState::PreparePhase)
		return;

	//����������ł����牽���s��Ȃ�
	if (m_IsDead)
		return;

	//HP�̒l����������
	m_pValueMap->writeMap(UnitStatsValues::Health, Value(getTransform().getLocalPosition(), 25.0f, m_Health));

	updateShield();

	move();
	rotate();

	updateAttack();
	updateTrailEffect();

	m_TargetCandidates.clear();
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

	const UnitStats* pUnitStats = m_pUnit->getUnitStats();
	m_Health = pUnitStats->m_MaxHealthPerObject;
	m_Shield = pUnitStats->m_MaxShieldPerObject;
	m_pValueMap = pValueMap;

	//�U���̓ǂݍ���
	for (auto& attacks : m_pUnit->getUnitStats()->getMainAttacks())
	{
		auto pEffectObj = new GameObject(getUser().getGameMediator());

		//�G�t�F�N�g����
		auto pAttackEffect = pEffectObj->addComponent<TestEffect_Beam>();
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

	//�ʏ�R���C�_�[
	m_pCollider = getUser().addComponent<CircleColliderB2>();
	m_pCollider->setTrigger(false);
	m_pCollider->setRadius(pUnitStats->m_ObjectSize.x);
	m_pCollider->setBodyType(b2BodyType::b2_dynamicBody);
	m_pCollider->setGroupIndex(0);

	//�g���K�[�p�R���C�_�[
	m_pTrigger = getUser().addComponent<CircleColliderB2>();
	m_pTrigger->setTrigger(true);

	//�˒������Ɠ����T�C�Y�ɂ��邽�߂ɃX�P�[���Ŋ���
	//�ł��˒������������U����o�^
	m_pLongestMainAttack = m_MainAttacks.at(0);
	float scaleX = pUnitStats->m_ObjectSize.x;
	m_pTrigger->setRadius(m_pLongestMainAttack->getAttackRange() / scaleX);
	m_pTrigger->setGroupIndex(-1);

	//�O�ՃG�t�F�N�g�R���|�[�l���g�ǉ�
	m_pCubeTrailEffect = getUser().addComponent<CubeTrailEffect>();
	m_pCubeTrailEffect->init(pEffectRenderHelper);
}

void UnitObject::setDestination(const Vec3& destination, bool isMoveCommand)
{
	m_Destination = destination;
	m_Destination.y = 0.0f;
}

void UnitObject::move()
{
	Vec3 diff = m_Destination - getTransform().getLocalPosition();
	diff.y = 0.0f;

	//���X�Ɍ���
	float ratio = std::fminf(1.0f, diff.length() / m_pUnit->getUnitStats()->m_Speed);
	float speed = Easing::easeOutExpo(ratio) * m_pUnit->getUnitStats()->m_Speed;
	m_pCollider->setVelocity(m_Direction.x * speed, m_Direction.z * speed);
}

void UnitObject::rotate()
{
	//��]���������߂�
	Vec3 diff = m_Destination - getTransform().getLocalPosition();
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
	if (m_IsDead) return;

	//�����`�[���Ȃ���s���Ȃ�
	if (getTeamID() == pUnitObject->getTeamID()) return;

	//�^�[�Q�b�g���ݒ�ς݂Ȃ�ǉ����Ȃ�
	if (m_pUnit->getTarget() != pUnitObject->getUnit() && m_pUnit->getTarget() != nullptr) return;

	//�^�[�Q�b�g���ǉ�
	m_TargetCandidates.insert(pUnitObject);

	m_pUnit->onEnterCombat(pUnitObject->getUnit());
}

void UnitObject::onTriggerExit(UnitObject* pUnitObject)
{
}

int UnitObject::getTeamID() const
{
	return m_pUnit->getTeamID();
}

bool UnitObject::isDead() const
{
	return m_IsDead;
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
	{
		m_IsDead = true;
		m_pCollider->setVelocity(0.0f, 0.0f);
		m_pCollider->setActive(false);
		m_pTrigger->setActive(false);
		m_pCubeTrailEffect->setActive(false);
	}
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

void UnitObject::updateShield()
{
	m_ShieldRegenTimer.update();

	//�V�[���h�񕜊J�n
	if (m_ShieldRegenTimer.isTime())
	{
		//�V�[���h�̍ő�l�𒴂��Ȃ��悤�ɉ�
		m_Shield = std::fminf(m_pUnit->getUnitStats()->m_MaxShieldPerObject,
			m_Shield + 1000.0f * GameDevice::getGameTime().getDeltaTime());
	}
}

void UnitObject::updateAttack()
{
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

	//�^�[�Q�b�g�ݒ�
	const Vec3& myPosition = getTransform().getLocalPosition();

	//�^�[�Q�b�g��␔���U���̐��������Ȃ��Ȃ�U�����W��������
	bool focusTarget = m_TargetCandidates.size() < m_MainAttacks.size();

	for (auto pAttack : m_MainAttacks)
	{
		UnitObject* pTarget = nullptr;

		for (auto pCandidate : m_TargetCandidates)
		{
			//����ł���Ȃ�ݒ肵�Ȃ�
			if (pCandidate->m_IsDead) continue;

			//�������v�Z
			float sqrDistance = myPosition.sqrDistance(pCandidate->getTransform().getLocalPosition());
			//�U���͈͓�������
			if (pAttack->getAttackRange() * pAttack->getAttackRange() < sqrDistance) continue;

			//�U���Ώۂ�ݒ�
			pTarget = pCandidate;
			break;
		}

		//�U���Ώۂ�ݒ�
		pAttack->setTarget(pTarget);

		//�ݒ肵���Ȃ烊�X�g����폜(�U�����W��������ꍇ�͂��Ȃ�)
		if (pTarget != nullptr &&
			!focusTarget)
			m_TargetCandidates.erase(pTarget);
	}
}

void UnitObject::updateTrailEffect()
{
	Vec3 diff = m_Destination - getTransform().getLocalPosition();

	//�ړI�n�ɒ������ꍇ�̓G�t�F�N�g��؂���return
	m_pCubeTrailEffect->setActive(diff.sqrLength() >= 0.5f);
}
