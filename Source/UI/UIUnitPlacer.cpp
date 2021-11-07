#include "UIUnitPlacer.h"

#include <filesystem>

#include "Actor\Base\GameObject.h"
#include "Component\Cursor.h"
#include "Device\GameDevice.h"
#include "Player\AIPlayer\AIPlayer.h"

#include "UI\UIUnitCard.h"
#include "UI\UIButton.h"
#include "Utility\JsonFileManager.h"
#include "Utility\ModelGameObjectHelper.h"
#include "Unit\Unit.h"

#include "Graphics\DX12\Material\DefaultMaterials.h"

void UIUnitPlacer::onStart()
{
}

void UIUnitPlacer::onUpdate()
{
}

void UIUnitPlacer::init(Cursor* pCursor,
	IPlayer* pPlayer,
	ValueMap* pValueMap,
	const std::unordered_map<std::string, UnitRenderHelper*>* pRenderHelpers)
{
	m_pCursor = pCursor;
	m_pRenderHelpers = pRenderHelpers;

	const float unitCardWidth = 60.0f;
	const float unitCardHeight = 96.0f;
	const float sliderHeight = 8.0f;
	const float spacePerUnitCard = 8.0f;

	std::vector<UnitStats> unitStatsList;
	auto& unitStatsManager = JsonFileManager<UnitStats>::getInstance();

	//Resources/UnitStatsの中を走査
	for (const auto& directory : std::filesystem::directory_iterator("Resources/UnitStats"))
	{
		const auto& path = directory.path();
		auto fileName = path.filename();
		//拡張子無しに変換
		std::string noExt = fileName.string().substr(0, fileName.string().size() - (size_t)5);
		unitStatsList.emplace_back(unitStatsManager.get(noExt));
	}

	const float positionOffset = (unitCardWidth + spacePerUnitCard) * (float)(unitStatsList.size() - 1) * 0.5f;

	IGameMediator* pGameMediator = getUser().getGameMediator();
	auto pModel = GameDevice::getModelManager().getModel("Sphere");
	for (int i = 0; i < (int)unitStatsList.size(); i++)
	{
		const auto& unitStats = unitStatsList.at(i);

		auto pButtonObj = new GameObject(getUser().getGameMediator());
		pButtonObj->getTransform().setLocalPosition(Vec3((unitCardWidth + spacePerUnitCard) * i - positionOffset, 300.0f, 1.0f));
		pButtonObj->getTransform().setLocalScale(Vec3(unitCardWidth, unitCardHeight, 1.0f));

		auto pButton = pButtonObj->addComponent<UIButton>();
		//ユニット配置関数を設定
		pButton->setOnMouseButtonDown(UIButton::MouseButtons::Left,
			[pGameMediator, pModel, pPlayer, unitStats, pValueMap, pRenderHelpers]()
			{
				//ユニット用オブジェクト生成
				auto pUnitObj = new GameObject(pGameMediator);
				//ユニット生成
				auto pUnit = pUnitObj->addComponent<Unit>();
				pUnit->init(pPlayer->getTeamID(), &unitStats, pValueMap, pRenderHelpers->at(unitStats.m_Name));
				//座標設定(TODO:プレイヤーの配置範囲の真ん中になるように変更する)
				pUnit->setPosition(Vec3(0.0f, 0.0f, 0.0f), 0.0f, 10);
				//ユニット登録
				pPlayer->addUnit(pUnit);
			}
		);

		//ステータス表示用親オブジェクト
		auto pStatsDisplayObj = new GameObject(getUser().getGameMediator());
		pStatsDisplayObj->setParent(&getUser());
		pStatsDisplayObj->getTransform().setLocalPosition(Vec3((unitCardWidth + spacePerUnitCard) * i - positionOffset, 300.0f, 1.0f));
		pStatsDisplayObj->getTransform().setLocalScale(Vec3(1.0f));

		//ユニットの画像(今は色だけ)
		auto pUnitCardObj = new GameObject(getUser().getGameMediator());
		pUnitCardObj->setParent(pStatsDisplayObj);
		pUnitCardObj->getTransform().setLocalPosition(Vec3(0.0f, 0.0f));
		pUnitCardObj->getTransform().setLocalScale(Vec3(unitCardWidth, unitCardHeight, 1.0f));
		auto pUnitCard = pUnitCardObj->addComponent<UIUnitCard>();
		pUnitCard->init(&unitStats);
	}
}
