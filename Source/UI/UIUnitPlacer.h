#pragma once
#include <vector>
#include "Component\Base\AbstractComponent.h"

class Cursor;
class IPlayer;
class Unit;
class UIUnitCard;
class ValueMap;
class InstancingMaterial;

class UIUnitPlacer
	: public AbstractComponent
{
public:
	virtual void onStart() override;
	virtual void onUpdate() override;

	void init(Cursor* pCursor, IPlayer* pPlayer, ValueMap* pValueMap, InstancingMaterial* pMaterial);

private:
	Cursor* m_pCursor;
	Unit* m_pGrabUnit;
	std::vector<UIUnitCard*> m_UnitCards;
};