#pragma once
#include <vector>

class Unit;

class IPlayer
{
public:
	virtual ~IPlayer() {}
	
	virtual void addUnit(Unit* pUnit) = 0;
	virtual const std::vector<Unit*>& getUnits() = 0;
	virtual void init(int teamNum, IPlayer* pOpponentPlayer) = 0;
};