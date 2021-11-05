#include "UIUnitCard.h"
#include "Actor\Base\GameObject.h"
#include "Component\Graphics\GUI\GUISpriteRenderer.h"
#include "Unit\UnitStats.h"

void UIUnitCard::onStart()
{
}

void UIUnitCard::onUpdate()
{
}

void UIUnitCard::init(const UnitStats* pUnitStats)
{
	//ユニットカード本体
	GUISpriteRenderer* pUnitCardRenderer = getUser().addComponent<GUISpriteRenderer>();
	pUnitCardRenderer->setTextureByName("BoxFill");
	pUnitCardRenderer->setColor(pUnitStats->m_DebugColor);
}
