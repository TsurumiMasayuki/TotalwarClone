#pragma once

class Unit;
class UnitContainer;
class ValueMap;

class IPlayer
{
public:
	virtual ~IPlayer() {}
	
	virtual void addUnit(Unit* pUnit) = 0;
	virtual UnitContainer* getUnitContainer() = 0;
	virtual void init(int teamNum, IPlayer* pOpponentPlayer, ValueMap* pValueMap) = 0;
};