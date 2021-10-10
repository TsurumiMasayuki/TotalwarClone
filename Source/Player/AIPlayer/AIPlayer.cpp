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
	//対戦相手がいないなら実行しない
	if (m_pOpponentPlayer == nullptr) return;

	auto& sortedUnits = m_pOpponentPlayer->getUnits()->getSortedUnits(UnitStatsValues::Health);
	auto pTarget = sortedUnits.at(0);
	auto pMyUnit = m_Units.getUnit(0);

	Vec3 unitPosition = pMyUnit->getTransform().getLocalPosition();
	Vec3 offset = Vec3(50.0f, 0.0f, 0.0f);
	Vec3 destinationCandidate;

	float increment = 0.0f;
	float valueSum = 0.0f;
	float angle = 0.0f;

	//目的地が見つかるまでループ
	while (true)
	{
		float sin = std::sinf(DirectX::XM_PI * increment);
		increment += 0.5f;

		//オフセットを回転
		Vec3 rotateOffset = Vec3(
			offset.x + offset.z * std::cosf(DirectX::XM_PI * 0.5f * sin),
			0.0f,
			offset.z + offset.x * std::sinf(DirectX::XM_PI * 0.5f * sin)
		);

		float value = m_pValueMap->readMap(UnitStatsValues::Health, unitPosition + rotateOffset);
		valueSum += value;

		//目的地を見つけた
		if (value < 100.0f)
		{
			destinationCandidate = unitPosition + rotateOffset;
			angle = 90.0f + 180.0f * sin;
			break;
		}

		//目的地が見つからなかったので後退
		if (increment == 1.0f)
		{
			destinationCandidate = unitPosition - offset;
			break;
		}
	}

	if (valueSum == 0.0f)
		//周りに障害物が無かったらターゲットに向けて直進
		pMyUnit->setTarget(pTarget);
	else
		//障害物があったら迂回
		pMyUnit->setDestination(destinationCandidate, angle);
}

void AIPlayer::addUnit(Unit* pUnit)
{
	m_Units.addUnit(pUnit);
}

UnitContainer* AIPlayer::getUnits()
{
	return &m_Units;
}

void AIPlayer::init(int teamNum, IPlayer* pOpponentPlayer, ValueMap* pValueMap)
{
	m_TeamNum = teamNum;
	m_pOpponentPlayer = pOpponentPlayer;
	m_pValueMap = pValueMap;
}