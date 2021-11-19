#include "Attack.h"
#include "Component\Utility\Transform.h"

#include "Unit\UnitObject.h"
#include "Unit\Attack\AttackStats.h"
#include "Effect\TestEffect_Beam.h"

Attack::Attack(UnitObject* pOwner, const AttackStats* pAttackStats, TestEffect_Beam* pEffect)
	: m_pAttackStats(pAttackStats),
	m_AttackTimer(pAttackStats->m_AttackInterval),
	m_IsActive(true),
	m_pOwner(pOwner),
	m_pTarget(nullptr),
	m_pEffect(pEffect),
	m_CurrentAngle(0.0f)
{
	m_pEffect->setWidth(0.5f * std::fmaxf(m_pAttackStats->m_Damage / 100.0f, 1.0f));
	m_pEffect->setTime(0.5f);
}

Attack::~Attack()
{
	//�G�t�F�N�g�I�u�W�F�N�g��j��
	m_pEffect->getUser().destroy();
}

void Attack::update()
{
	m_AttackTimer.update();

	if (!m_IsActive || m_pTarget == nullptr) return;

	//�U���Ԋu�̕��̎��Ԃ��o�����Ȃ�U��
	if (m_AttackTimer.isTime())
	{
		attackTarget();
		m_AttackTimer.reset();
	}
}

void Attack::setActive(bool isActive)
{
	m_IsActive = isActive;
}

bool Attack::getActive() const
{
	return m_IsActive;
}

void Attack::setTarget(UnitObject* pTarget)
{
	m_pTarget = pTarget;
}

UnitObject* Attack::getTarget() const
{
	return m_pTarget;
}

float Attack::getAttackRange() const
{
	return m_pAttackStats->m_AttackRange;
}

void Attack::attackTarget()
{
	m_pTarget->takeDamage(m_pAttackStats->m_Damage);

	//�G�t�F�N�g�Đ�
	m_pEffect->setBeginPos(m_pOwner->getTransform().getLocalPosition());
	m_pEffect->setEndPos(m_pTarget->getTransform().getLocalPosition());

	m_pEffect->playEffect();
}

void Attack::trackTarget()
{
	//����������Ă���Ȃ�return;
	if (!m_IsActive) return;
	//�ǐՂ��Ȃ��Ȃ�return
	if (!m_pAttackStats->m_IsTrackTarget) return;
}
