#pragma once
#include <vector>
#include <unordered_map>
#include "Component\Base\AbstractComponent.h"

class Cursor;
class Player;
class UnitSelector;
class Stage;
class UIUnitCard;
class UIUnitInfo;
class ValueMap;
class UnitRenderHelper;
class EffectRenderHelper;

class UIUnitPlacer
	: public AbstractComponent
{
public:
	virtual void onStart() override;
	virtual void onUpdate() override;

	void init(Cursor* pCursor,
		Player* pPlayer,
		UnitSelector* pUnitSelector,
		const Stage& stage,
		UIUnitInfo* pUnitInfo,
		ValueMap* pValueMap,
		const std::unordered_map<std::string, UnitRenderHelper*>* pRenderHelpers,
		EffectRenderHelper* pEffectRenderHelper);

private:
	Cursor* m_pCursor;
	std::vector<UIUnitCard*> m_UnitCards;
	const std::unordered_map<std::string, UnitRenderHelper*>* m_pRenderHelpers;
};