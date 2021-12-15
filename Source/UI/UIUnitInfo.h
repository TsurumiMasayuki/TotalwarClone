#pragma once
#include <vector>
#include "Component\Base\AbstractComponent.h"
#include "Component\Graphics\D2DTextRenderer.h"

#include "Unit/UnitStats.h"

class Unit;
class UISlider;

class UIUnitInfo
	: public AbstractComponent
{
public:
	virtual void onStart() override;
	virtual void onUpdate() override;

	//ユニット情報を表示
	void setUnit(Unit* pUnit);
	//ユニット情報を直接表示
	void setUnitInfoDirect(const UnitStats* pUnitStats);

private:
	void initUIObjects();

private:
	Unit* m_pUnit;
	D2DTextRenderer* m_pName;
	D2DTextRenderer* m_pDescription;
	D2DTextRenderer* m_pCost;

	D2DTextRenderer* m_pHealth;
	D2DTextRenderer* m_pHealthValue;
	UISlider* m_pHealthSlider;

	D2DTextRenderer* m_pSpeed;
	UISlider* m_pSpeedSlider;
};