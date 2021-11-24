#include "Player.h"
#include "Actor\Base\GameObject.h"
#include "Component\Graphics\D2DTextRenderer.h"

#include "Unit\Unit.h"

void Player::onStart()
{
}

void Player::onUpdate()
{
	m_pEnergyText->setText(std::to_wstring(m_CurrentEnergy));
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

	auto pEnergyObj = new GameObject(getUser().getGameMediator());
	pEnergyObj->setParent(&getUser());
	pEnergyObj->getTransform().setLocalPosition(Vec3(500.0f, 300.0f));
	pEnergyObj->getTransform().setLocalScale(Vec3(1.0f, 1.0f, 1.0f));
	m_pEnergyText = pEnergyObj->addComponent<D2DTextRenderer>();
	m_pEnergyText->setFont(L"Meiryo UI",
		L"ja-jp",
		50.0f
	);
	m_pEnergyText->setFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_MEDIUM);
	m_pEnergyText->setFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
	m_pEnergyText->setFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
	m_pEnergyText->setColor(Color(0.0f, 0.0f, 0.0f, 1.0f));
	m_pEnergyText->setTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
	m_pEnergyText->setParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	m_pEnergyText->setTextRect(0.0f, 0.0f, 1280.0f, 720.0f);
	m_pEnergyText->setText(std::to_wstring(m_CurrentEnergy));
}

bool Player::isDefeat()
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

void Player::setEnergy(int energy)
{
	m_CurrentEnergy = energy;
}

int Player::getEnergy() const
{
	return m_CurrentEnergy;
}
