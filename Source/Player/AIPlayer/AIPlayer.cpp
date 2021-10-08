#include "AIPlayer.h"
#include "Unit\Unit.h"

AIPlayer::AIPlayer()
{
}

AIPlayer::~AIPlayer()
{
}

void AIPlayer::onStart()
{
}

void AIPlayer::onUpdate()
{
    //‘Îí‘ŠŽè‚ª‚¢‚È‚¢‚È‚çŽÀs‚µ‚È‚¢
    if (m_pOpponentPlayer == nullptr) return;

    auto pTarget = m_pOpponentPlayer->getUnits().at(0);
    m_Units.at(0)->setTarget(pTarget);
}

void AIPlayer::addUnit(Unit* pUnit)
{
    m_Units.emplace_back(pUnit);
}

const std::vector<Unit*>& AIPlayer::getUnits()
{
    return m_Units;
}

void AIPlayer::init(int teamNum, IPlayer* pOpponentPlayer)
{
    m_TeamNum = teamNum;
    m_pOpponentPlayer = pOpponentPlayer;
}
