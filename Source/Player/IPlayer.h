#pragma once

class Unit;
class UnitContainer;
class ValueMap;

class IPlayer
{
public:
	virtual ~IPlayer() {}
	
	virtual int getTeamID() = 0;
	virtual void addUnit(Unit* pUnit) = 0;
	virtual void removeUnit(Unit* pUnit) = 0;
	virtual UnitContainer* getUnitContainer() = 0;
	virtual void init(int teamNum, IPlayer* pOpponentPlayer, ValueMap* pValueMap) = 0;
	virtual bool isDefeat() = 0;
};