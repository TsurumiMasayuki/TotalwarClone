#include "UIUnitCard.h"
#include "Actor\Base\GameObject.h"
#include "Component\Graphics\GUI\GUISpriteRenderer.h"
#include "Unit\Unit.h"
#include "Unit\UnitStats.h"

void UIUnitCard::onStart()
{
}

void UIUnitCard::onUpdate()
{
}

void UIUnitCard::init(Unit* pUnit)
{
	m_pUnit = pUnit;
	auto pUnitStats = m_pUnit->getUnitStats();

	//���j�b�g�J�[�h�{��
	GUISpriteRenderer* pUnitCardRenderer = getUser().addComponent<GUISpriteRenderer>();
	pUnitCardRenderer->setTextureByName("BoxFill");
	pUnitCardRenderer->setColor(pUnitStats->m_DebugColor);
}
