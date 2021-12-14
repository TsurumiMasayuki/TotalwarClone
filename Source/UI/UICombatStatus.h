#pragma once
#include "Component\Base\AbstractComponent.h"

class Unit;
class GUISpriteRenderer;

class UICombatStatus
	: public AbstractComponent
{
public:
	virtual void onStart() override;
	virtual void onUpdate() override;

	void init(Unit* pUnit);

private:
	Unit* m_pUnit;
	GUISpriteRenderer* m_pRenderer;
};