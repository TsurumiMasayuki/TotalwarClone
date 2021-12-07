#include "AIPlayer.h"
#include "Component\Utility\Transform.h"

#include "GameState.h"

#include "AI\Controller\SimpleMoveController.h"
#include "AI\Controller\SimpleCombatController.h"
#include "AI\Controller\MarchController.h"

#include "Unit\Unit.h"

Game::GameState Game::g_GameState;

AIPlayer::AIPlayer()
{
}

AIPlayer::~AIPlayer()
{
}

void AIPlayer::onStart()
{
	m_ControllerUpdated = false;
}

void AIPlayer::onUpdate()
{
	//対戦相手がいないなら実行しない
	if (m_pOpponentPlayer == nullptr) return;

	if (Game::g_GameState == Game::GameState::CombatPhaseBegin)
	{
		Unit* pEnemyCenterUnit = m_pOpponentPlayer->getUnitContainer()->getCenterUnit();

		//一番遅いユニットを取得
		const auto& units = m_Units.getSortedUnits(UnitStatsValues::Speed);
		m_pBaseUnit = units.at(0);

		//コントローラーを生成して登録
		m_Controllers.at(m_pBaseUnit) = new SimpleMoveController(m_pBaseUnit, pEnemyCenterUnit, m_pBaseUnit->getUnitStats()->m_DefaultWidth);
		const Vec3& slowestUnitPos = m_pBaseUnit->getTransform().getLocalPosition();

		//一番遅いユニット以外に適用
		for (auto pUnit : units)
		{
			if (m_Controllers.at(pUnit))
				continue;

			//基準にするオブジェクトからの相対座標を算出
			const Vec3& relativePos = pUnit->getTransform().getLocalPosition() - slowestUnitPos;
			//コントローラーを生成
			m_Controllers.at(pUnit) = new MarchController(pUnit, m_pBaseUnit, relativePos, pUnit->getUnitStats()->m_DefaultWidth);
		}

		//初期化処理
		for (auto& pair : m_Controllers)
		{
			pair.second->start();
		}
	}

	if (Game::g_GameState == Game::GameState::CombatPhase)
	{
		//ユニットが戦闘中ならコントローラーを更新
		for (auto pUnit : m_Units.getUnits())
		{
			//移動の中心になるユニットが死亡していたら戦闘AIに切り替える
			if (!pUnit->isInCombat() &&
				m_pBaseUnit->getObjectCount() > 0) continue;

			//戦闘中ならcontinue
			if (m_ControllerUpdated) continue;

			for (auto pUnit2 : m_Units.getUnits())
			{
				setNewController(pUnit2, new SimpleCombatController(pUnit2, m_pOpponentPlayer));
			}

			m_ControllerUpdated = true;
		}

		//更新処理
		for (auto& pair : m_Controllers)
		{
			pair.second->update();
		}
	}
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

bool AIPlayer::isDefeat()
{
	bool isDefeat = true;

	const auto& units = m_Units.getUnits();

	for (const auto pUnit : units)
	{
		if (pUnit->getObjectCount() == 0) continue;

		isDefeat = false;
		break;
	}

	return isDefeat;
}

void AIPlayer::setNewController(Unit* pUnit, AbstractController* pController)
{
	//nullでないならdelete
	if (m_Controllers.at(pUnit) != nullptr)
		delete m_Controllers.at(pUnit);

	//新しいコントローラーをセット
	m_Controllers.at(pUnit) = pController;
}
