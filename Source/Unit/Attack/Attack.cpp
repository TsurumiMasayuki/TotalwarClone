#include "Attack.h"
#include "Unit\UnitObject.h"

Attack::Attack(float damage,
	float attackInterval,
	float attackRange,
	float trackSpeed,
	bool isTrackTarget,
	int maxAttackCount,
	bool isInfiniteAttack,
	UnitObject* pOwner)
	: m_Damage(damage),
	m_AttackInterval(attackInterval),
	m_AttackRange(attackRange),
	m_TrackSpeed(trackSpeed),
	m_IsTrackTarget(isTrackTarget),
	m_MaxAttackCount(maxAttackCount),
	m_IsInfiniteAttack(isInfiniteAttack),
	m_pAttackEffect(nullptr),
	m_AttackTimer(attackInterval),
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
	if (!m_IsActive) return;

	m_AttackTimer.update();

	//攻撃間隔の分の時間が経ったなら攻撃
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
	return m_AttackRange;
}

void Attack::attackTarget()
{
	if (m_pTarget == nullptr) return;

	//エフェクトの実行などする(予定)
	m_pTarget->takeDamage(m_Damage);
}

void Attack::trackTarget()
{
	//無効化されているならreturn;
	if (!m_IsActive) return;
	//追跡しないならreturn
	if (!m_IsTrackTarget) return;



}
