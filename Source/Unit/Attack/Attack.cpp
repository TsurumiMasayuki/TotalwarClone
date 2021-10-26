#include "Attack.h"
#include "Unit\UnitObject.h"
#include "Unit\Attack\AttackStats.h"

Attack::Attack(UnitObject* pOwner, const AttackStats* pAttackStats)
	: m_pAttackStats(pAttackStats),
	m_AttackTimer(pAttackStats->m_AttackInterval),
	m_IsActive(true),
	m_pOwner(pOwner),
	m_pTarget(nullptr),
	m_CurrentAngle(0.0f)
{
}

Attack::~Attack()
{
}

void Attack::update()
{
	if (!m_IsActive || m_pTarget == nullptr) return;

	m_AttackTimer.update();

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
	if (m_pTarget == nullptr ||
		m_pTarget->getState() == UnitObject::State::Dead)
		return;

	//�G�t�F�N�g�̎��s�Ȃǂ���(�\��)
	m_pTarget->takeDamage(m_pAttackStats->m_Damage);
}

void Attack::trackTarget()
{
	//����������Ă���Ȃ�return;
	if (!m_IsActive) return;
	//�ǐՂ��Ȃ��Ȃ�return
	if (!m_pAttackStats->m_IsTrackTarget) return;



}
