#include "Player.h"

void Player::onStart()
{
}

void Player::onUpdate()
{
}

void Player::addUnit(Unit* pUnit)
{
	m_Units.addUnit(pUnit);
}

UnitContainer* Player::getUnitContainer()
{
	return &m_Units;
}

void Player::init(int teamNum, IPlayer* pOpponentPlayer, ValueMap* pValueMap)
{
	m_TeamNum = teamNum;
	m_pOpponentPlayer = pOpponentPlayer;
	m_pValueMap = pValueMap;
}