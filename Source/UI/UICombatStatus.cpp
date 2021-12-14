#include "UICombatStatus.h"

#include "Actor\Base\GameObject.h"
#include "Component\Graphics\GUI\GUISpriteRenderer.h"
#include "Math\Vec3.h"

#include "Unit\Unit.h"
#include "Unit\UnitStats.h"

void UICombatStatus::onStart()
{
}

void UICombatStatus::onUpdate()
{
	Unit* pTarget = m_pUnit->getTarget();

	//�^�[�Q�b�g������or���g���S�ŏ�ԂȂ�
	if (pTarget == nullptr ||
		m_pUnit->getObjectCount() == 0)
	{
		//��\���ɂ���
		m_pRenderer->setColor(Color(0.0f, 0.0f, 0.0f, 0.0f));
		return;
	}

	//���j�b�g��HP
	float unitHealth = m_pUnit->getHealth();
	//�^�[�Q�b�g��HP
	float targetHealth = pTarget->getHealth();

	float diff = std::fabsf(unitHealth - targetHealth);

	Color color;
	//�̗͂��r���ĝh�R��ԂȂ�I�����W�F
	if (diff < 500.0f)
	{
		color = Color(DirectX::Colors::Orange);
	}

	//�̗͂��r���Ă����瑤���s���Ȃ�ԐF
	if (unitHealth < targetHealth)
	{
		color = Color(DirectX::Colors::Red);
	}

	//�̗͂��r���Ă����瑤���L���Ȃ�ԐF
	if (unitHealth > targetHealth)
	{
		color = Color(DirectX::Colors::Green);
	}

	//�F��ݒ�
	m_pRenderer->setColor(color);
}

void UICombatStatus::init(Unit* pUnit)
{
	m_pUnit = pUnit;

	m_pRenderer = getUser().addComponent<GUISpriteRenderer>();
	m_pRenderer->setTextureByName("CombatIcon");
	//��\���ɂ���
	m_pRenderer->setColor(Color(0.0f, 0.0f, 0.0f, 0.0f));
}
