#pragma once
#include <vector>
#include "Component\Base\AbstractComponent.h"

class IPlayer;

class UIUnitList
	: public AbstractComponent
{
public:
	virtual void onStart() override;
	virtual void onUpdate() override;

	void init(IPlayer* pPlayer, float spacePerUnitCard);

private:
	IPlayer* m_pPlayer;
};