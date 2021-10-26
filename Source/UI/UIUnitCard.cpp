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
	//ユニットカード本体
	GUISpriteRenderer* pUnitCardRenderer = getUser().addComponent<GUISpriteRenderer>();
	pUnitCardRenderer->setTextureByName("BoxFill");
	pUnitCardRenderer->setColor(m_pUnit->getUnitStats()->m_Color);
}
