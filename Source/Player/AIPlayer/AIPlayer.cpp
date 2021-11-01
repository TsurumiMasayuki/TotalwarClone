#include "AIPlayer.h"
#include "Unit\Unit.h"
#include "AI\Controller\SimpleMoveController.h"
#include "AI\Controller\MarchController.h"

AIPlayer::AIPlayer()
{
}

AIPlayer::~AIPlayer()
{
}

void AIPlayer::onStart()
{
	const auto& opponentUnits = m_pOpponentPlayer->getUnitContainer()->getUnits();

	//一番遅いユニットを見つける
	const auto& units = m_Units.getSortedUnits(UnitStatsValues::Speed);
	auto pSlowestUnit = units.at(0);

	//コントローラーを生成して登録
	m_Controllers.at(pSlowestUnit) = new SimpleMoveController(pSlowestUnit, opponentUnits.at(0), 10);

	//一番遅いユニット以外に適用
	for (int i = 1; i < (int)units.size(); i++)
	{
		//初期位置を相対座標として取得
		const Vec3& relativePos = units.at(i)->getTransform().getLocalPosition();
		//コントローラーを生成
		m_Controllers.at(units.at(i)) = new MarchController(units.at(i), this, relativePos, 5);
	}

	//初期化処理
	for (auto& pair : m_Controllers)
	{
		pair.second->start();
	}
}

void AIPlayer::onUpdate()
{
	//対戦相手がいないなら実行しない
	if (m_pOpponentPlayer == nullptr) return;

	//更新処理
	for (auto& pair : m_Controllers)
	{
		pair.second->update();
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

void AIPlayer::addUnit(Unit* pUnit)
{
	//ユニットを追加
	m_Units.addUnit(pUnit);
	//コントローラーのスロット追加
	m_Controllers.emplace(pUnit, nullptr);
}

UnitContainer* AIPlayer::getUnitContainer()
{
	return &m_Units;
}

void AIPlayer::init(int teamNum, IPlayer* pOpponentPlayer, ValueMap* pValueMap)
{
	m_TeamNum = teamNum;
	m_pOpponentPlayer = pOpponentPlayer;
	m_pValueMap = pValueMap;
}