#include "Player.h"
#include "Unit\Unit.h"

void Player::onStart()
{
	m_CurrentEnergy = 5000;
}

void Player::onUpdate()
{
}

int Player::getTeamID()
{
	return m_TeamID;
}

void Player::addUnit(Unit* pUnit)
{
	m_Units.addUnit(pUnit);
	m_CurrentEnergy -= pUnit->getUnitStats()->m_EnergyCost;
}

void Player::removeUnit(Unit* pUnit)
{
	m_Units.removeUnit(pUnit);
	m_CurrentEnergy += pUnit->getUnitStats()->m_EnergyCost;
}

UnitContainer* Player::getUnitContainer()
{
	return &m_Units;
}

void Player::init(int teamNum, IPlayer* pOpponentPlayer, ValueMap* pValueMap)
{
	m_TeamID = teamNum;
	m_pOpponentPlayer = pOpponentPlayer;
	m_pValueMap = pValueMap;
}

int Player::getEnergy() const
{
	return m_CurrentEnergy;
}
