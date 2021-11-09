#include "AIPlayer.h"
#include "Unit\Unit.h"
#include "AI\Controller\SimpleMoveController.h"
#include "AI\Controller\MarchController.h"
#include "Component\Utility\Transform.h"
#include "GameState.h"

Game::GameState Game::g_GameState;

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

	if (Game::g_GameState == Game::GameState::CombatPhaseBegin)
	{
		Unit* pEnemyCenterUnit = m_pOpponentPlayer->getUnitContainer()->getCenterUnit();

		//真ん中のユニットを取得
		const auto& units = m_Units.getSortedUnits(UnitStatsValues::Speed);
		auto pBaseUnit = getUnitContainer()->getCenterUnit();

		//コントローラーを生成して登録
		m_Controllers.at(pBaseUnit) = new SimpleMoveController(pBaseUnit, pEnemyCenterUnit, pBaseUnit->getUnitStats()->m_DefaultWidth);
		const Vec3& slowestUnitPos = pBaseUnit->getTransform().getLocalPosition();

		//一番遅いユニット以外に適用
		for (auto pUnit : units)
		{
			if (m_Controllers.at(pUnit))
				continue;

			//基準にするオブジェクトからの相対座標を算出
			const Vec3& relativePos = pUnit->getTransform().getLocalPosition() - slowestUnitPos;
			//コントローラーを生成
			m_Controllers.at(pUnit) = new MarchController(pUnit, this, relativePos, pUnit->getUnitStats()->m_DefaultWidth);
		}

		//初期化処理
		for (auto& pair : m_Controllers)
		{
			pair.second->start();
		}
	}

	if (Game::g_GameState == Game::GameState::CombatPhase)
	{
		//更新処理
		for (auto& pair : m_Controllers)
		{
			pair.second->update();
		}
	}

	//auto& sortedUnits = m_pOpponentPlayer->getUnitContainer()->getSortedUnits(UnitStatsValues::Health);
	//auto pTarget = sortedUnits.at(0);
	//auto pMyUnit = m_Units.getUnit(0);

	//Vec3 unitPosition = pMyUnit->getTransform().getLocalPosition();
	//Vec3 offset = Vec3(50.0f, 0.0f, 0.0f);
	//Vec3 destinationCandidate;

	//float increment = 0.0f;
	//float valueSum = 0.0f;
	//float angle = 0.0f;

	////目的地が見つかるまでループ
	//while (true)
	//{
	//	float sin = std::sinf(DirectX::XM_PI * increment);
	//	increment += 0.5f;

	//	//オフセットを回転
	//	Vec3 rotateOffset = Vec3(
	//		offset.x + offset.z * std::cosf(DirectX::XM_PI * 0.5f * sin),
	//		0.0f,
	//		offset.z + offset.x * std::sinf(DirectX::XM_PI * 0.5f * sin)
	//	);

	//	float value = m_pValueMap->readMap(UnitStatsValues::Health, unitPosition + rotateOffset);
	//	valueSum += value;

	//	//目的地を見つけた
	//	if (value < 100.0f)
	//	{
	//		destinationCandidate = unitPosition + rotateOffset;
	//		angle = 90.0f + 180.0f * sin;
	//		break;
	//	}

	//	//目的地が見つからなかったので後退
	//	if (increment == 1.0f)
	//	{
	//		destinationCandidate = unitPosition - offset;
	//		break;
	//	}
	//}

	//if (valueSum == 0.0f)
	//	//周りに障害物が無かったらターゲットに向けて直進
	//	pMyUnit->setTarget(pTarget);
	//else
	//	//障害物があったら迂回
	//	pMyUnit->setDestination(destinationCandidate, angle, 10);
}

void AIPlayer::onDestroy()
{
	for (auto pair : m_Controllers)
	{
		delete pair.second;
	}
}

int AIPlayer::getTeamID()
{
	return m_TeamID;
}

void AIPlayer::addUnit(Unit* pUnit)
{
	//ユニットを追加
	m_Units.addUnit(pUnit);
	//コントローラーのスロット追加
	m_Controllers.emplace(pUnit, nullptr);
}

void AIPlayer::removeUnit(Unit* pUnit)
{
	//ユニットを削除
	m_Units.removeUnit(pUnit);
}

UnitContainer* AIPlayer::getUnitContainer()
{
	return &m_Units;
}

void AIPlayer::init(int teamNum, IPlayer* pOpponentPlayer, ValueMap* pValueMap)
{
	m_TeamID = teamNum;
	m_pOpponentPlayer = pOpponentPlayer;
	m_pValueMap = pValueMap;
}