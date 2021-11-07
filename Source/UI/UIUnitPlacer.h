#pragma once
#include <vector>
#include <unordered_map>
#include "Component\Base\AbstractComponent.h"

class Cursor;
class IPlayer;
class Unit;
class UIUnitCard;
class ValueMap;
class UnitRenderHelper;

class UIUnitPlacer
	: public AbstractComponent
{
public:
	virtual void onStart() override;
	virtual void onUpdate() override;

	void init(Cursor* pCursor, IPlayer* pPlayer, ValueMap* pValueMap, const std::unordered_map<std::string, UnitRenderHelper*>* pRenderHelpers);

private:
	Cursor* m_pCursor;
	Unit* m_pGrabUnit;
	std::vector<UIUnitCard*> m_UnitCards;
	const std::unordered_map<std::string, UnitRenderHelper*>* m_pRenderHelpers;
};