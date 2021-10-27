#include "UIUnitList.h"
#include "Player\IPlayer.h"
#include "Unit\Unit.h"
#include "Unit\UnitContainer.h"
#include "UI\UIUnitCard.h"
#include "UI\UISlider.h"
#include "UI\UISliderHelper.h"

void UIUnitList::onStart()
{
}

void UIUnitList::onUpdate()
{
}

void UIUnitList::init(IPlayer* pPlayer, float spacePerUnitCard)
{
	const float unitCardWidth = 60.0f;
	const float unitCardHeight = 96.0f;
	const float sliderHeight = 8.0f;

	m_pPlayer = pPlayer;
	const auto& units = m_pPlayer->getUnitContainer()->getUnits();
	const float positionOffset = (unitCardWidth + spacePerUnitCard) * (float)(units.size() - 1) * 0.5f;

	for (int i = 0; i < (int)units.size(); i++)
	{
		auto pUnit = units.at(i);

		//ステータス表示用親オブジェクト
		auto pStatsDisplayObj = new GameObject(getUser().getGameMediator());
		pStatsDisplayObj->setParent(&getUser());
		pStatsDisplayObj->getTransform().setLocalPosition(Vec3((unitCardWidth + spacePerUnitCard) * i - positionOffset, -300.0f, 1.0f));
		pStatsDisplayObj->getTransform().setLocalScale(Vec3(1.0f));

		//ユニットの画像(今は色だけ)
		auto pUnitCardObj = new GameObject(getUser().getGameMediator());
		pUnitCardObj->setParent(pStatsDisplayObj);
		pUnitCardObj->getTransform().setLocalPosition(Vec3(0.0f, 0.0f));
		pUnitCardObj->getTransform().setLocalScale(Vec3(unitCardWidth, unitCardHeight, 1.0f));
		auto pUnitCard = pUnitCardObj->addComponent<UIUnitCard>();
		pUnitCard->init(pUnit);

		auto pHealthObj = new GameObject(getUser().getGameMediator());
		pHealthObj->setParent(pStatsDisplayObj);
		pHealthObj->getTransform().setLocalPosition(Vec3(0.0f, unitCardHeight * 0.5f + sliderHeight * 0.5f, 1.0f));
		pHealthObj->getTransform().setLocalScale(Vec3(unitCardWidth, sliderHeight, 1.0f));

		//ユニットの体力表示
		auto pHealthSlider = pHealthObj->addComponent<UISlider>();
		pHealthSlider->setWidth(1.0f);
		pHealthSlider->setDirection(UISlider::Direction::RIGHT);
		pHealthSlider->setColor(Color(DirectX::Colors::Green));
		pHealthSlider->setCurrentValue(pUnit->getHealth());
		pHealthSlider->setMaxValue(pUnit->getHealth());
		pHealthSlider->setTextureByName("BoxFill");

		//スライダー補助(HP)
		auto pHealthHelper = pHealthObj->addComponent<UISliderHelper<Unit>>();
		pHealthHelper->init(pHealthSlider, pUnit, [](Unit* pUnit)
			{
				return pUnit->getHealth();
			}
		);

		//シールドがあるなら
		if (pUnit->getShield() > 0.0f)
		{
			auto pShieldObj = new GameObject(getUser().getGameMediator());
			pShieldObj->setParent(pStatsDisplayObj);
			pShieldObj->getTransform().setLocalPosition(Vec3(0.0f, unitCardHeight * 0.5f + sliderHeight * 0.5f));
			pShieldObj->getTransform().setLocalScale(Vec3(unitCardWidth, sliderHeight, 1.0f));

			//ユニットのシールド表示
			auto pShieldSlider = pShieldObj->addComponent<UISlider>();
			pShieldSlider->setWidth(1.0f);
			pShieldSlider->setDirection(UISlider::Direction::RIGHT);
			pShieldSlider->setColor(Color(DirectX::Colors::Cyan));
			pShieldSlider->setCurrentValue(pUnit->getShield());
			pShieldSlider->setMaxValue(pUnit->getShield());
			pShieldSlider->setTextureByName("BoxFill");

			//スライダー補助(シールド)
			auto pShieldHelper = pShieldObj->addComponent<UISliderHelper<Unit>>();
			pShieldHelper->init(pShieldSlider, pUnit, [](Unit* pUnit)
				{
					return pUnit->getShield();
				}
			);
		}
	}
}
