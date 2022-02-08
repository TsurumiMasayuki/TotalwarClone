#include "UIUnitInfo.h"
#include "Actor\Base\GameObject.h"
#include "Component\Graphics\GUI\GUISpriteRenderer.h"
#include "Def\Screen.h"
#include "Utility\StringUtility.h"

#include "Unit\Unit.h"
#include "UI\UISlider.h"

#include "GameState.h"

void UIUnitInfo::onStart()
{
}

void UIUnitInfo::onUpdate()
{
	if (Game::g_GameState == Game::GameState::Result) 
	{
		getUser().setActive(false);
	}

	if (m_pUnit == nullptr) return;

	m_pHealthValue->setText(std::to_wstring((int)m_pUnit->getHealth()));
	m_pHealthSlider->setCurrentValue(m_pUnit->getHealth());
}

void UIUnitInfo::setUnit(Unit* pUnit)
{
	m_pUnit = pUnit;
	auto pUnitStats = m_pUnit->getUnitStats();

	if (m_pName == nullptr)
		initUIObjects();

	m_pName->setText(StringUtility::ToWString(pUnitStats->m_DisplayName));

	//HPのテキスト設定
	m_pHealthValue->setText(std::to_wstring((int)m_pUnit->getHealth()));
	//HPバー設定
	m_pHealthSlider->setCurrentValue(m_pUnit->getHealth());
	m_pHealthSlider->setMaxValue(pUnitStats->m_MaxHealthPerObject * pUnitStats->m_ObjectCount);

	m_pCost->setText(L"コスト：" + std::to_wstring(pUnitStats->m_EnergyCost));

	m_pSpeedSlider->setCurrentValue(pUnitStats->m_Speed);
	m_pSpeedSlider->setMaxValue(25.0f);
}

void UIUnitInfo::setUnitInfoDirect(const UnitStats* pUnitStats)
{
	if (m_pName == nullptr)
		initUIObjects();

	m_pName->setText(StringUtility::ToWString(pUnitStats->m_DisplayName));

	float health = pUnitStats->m_MaxHealthPerObject * pUnitStats->m_ObjectCount;

	//HPのテキスト設定
	m_pHealthValue->setText(std::to_wstring((int)health));
	//HPバー設定
	m_pHealthSlider->setCurrentValue(health);
	m_pHealthSlider->setMaxValue(health);

	m_pCost->setText(L"コスト：" + std::to_wstring(pUnitStats->m_EnergyCost));

	m_pSpeedSlider->setCurrentValue(pUnitStats->m_Speed);
	m_pSpeedSlider->setMaxValue(25.0f);
}

void UIUnitInfo::initUIObjects()
{
	auto pMediator = getUser().getGameMediator();
	const float fontSize = 12.0f;

	//名前表示用
	{
		auto pObj = new GameObject(pMediator);
		pObj->setParent(&getUser());
		pObj->getTransform().setLocalPosition(Vec3(500.0f, 120.0f));
		m_pName = pObj->addComponent<D2DTextRenderer>();
		m_pName->setText(L"名前");
		m_pName->setFont(L"Meiryo UI", L"ja-jp", fontSize);
		m_pName->setFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_MEDIUM);
		m_pName->setFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
		m_pName->setFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
		m_pName->setTextRect(0.0f, 0.0f, WindowWidth, WindowHeight);
		m_pName->setColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
	}

	//説明表示用
	{
		auto pObj = new GameObject(pMediator);
		pObj->setParent(&getUser());
		pObj->getTransform().setLocalPosition(Vec3(400.0f, 60.0f));
		m_pDescription = pObj->addComponent<D2DTextRenderer>();
		m_pDescription->setText(L"説明");
		m_pDescription->setFont(L"Meiryo UI", L"ja-jp", fontSize);
		m_pDescription->setFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_MEDIUM);
		m_pDescription->setFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
		m_pDescription->setFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
		m_pDescription->setTextRect(0.0f, 0.0f, WindowWidth, WindowHeight);
		m_pDescription->setColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
	}

	//HP表示用
	{
		auto pObj = new GameObject(pMediator);
		pObj->setParent(&getUser());
		pObj->getTransform().setLocalPosition(Vec3(400.0f, 0.0f));
		m_pHealth = pObj->addComponent<D2DTextRenderer>();
		m_pHealth->setText(L"HP");
		m_pHealth->setFont(L"Meiryo UI", L"ja-jp", fontSize);
		m_pHealth->setFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_MEDIUM);
		m_pHealth->setFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
		m_pHealth->setFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
		m_pHealth->setTextRect(0.0f, 0.0f, WindowWidth, WindowHeight);
		m_pHealth->setColor(Color(1.0f, 1.0f, 1.0f, 1.0f));

		auto pHealthValueObj = new GameObject(pMediator);
		pHealthValueObj->setParent(&getUser());
		pHealthValueObj->getTransform().setLocalPosition(Vec3(500.0f, 0.0f));
		m_pHealthValue = pHealthValueObj->addComponent<D2DTextRenderer>();
		m_pHealthValue->setText(L"");
		m_pHealthValue->setFont(L"Meiryo UI", L"ja-jp", fontSize);
		m_pHealthValue->setFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_MEDIUM);
		m_pHealthValue->setFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
		m_pHealthValue->setFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
		m_pHealthValue->setTextRect(0.0f, 0.0f, WindowWidth, WindowHeight);
		m_pHealthValue->setColor(Color(1.0f, 1.0f, 1.0f, 1.0f));

		auto pSliderObj = new GameObject(pMediator);
		pSliderObj->getTransform().setLocalPosition(Vec3(500.0f, 0.0f, 1.0f));
		pSliderObj->getTransform().setLocalScale(Vec3(100.0f, 1.0f, 1.0f));
		pSliderObj->setParent(&getUser());
		m_pHealthSlider = pSliderObj->addComponent<UISlider>();
		m_pHealthSlider->setDirection(UISlider::Direction::RIGHT);
		m_pHealthSlider->setWidth(16.0f);
		m_pHealthSlider->setColor(Color(DirectX::Colors::Red));
	}

	//コスト表示用
	{
		auto pObj = new GameObject(pMediator);
		pObj->setParent(&getUser());
		pObj->getTransform().setLocalPosition(Vec3(400.0f, -60.0f));
		m_pCost = pObj->addComponent<D2DTextRenderer>();
		m_pCost->setText(L"コスト");
		m_pCost->setFont(L"Meiryo UI", L"ja-jp", fontSize);
		m_pCost->setFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_MEDIUM);
		m_pCost->setFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
		m_pCost->setFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
		m_pCost->setTextRect(0.0f, 0.0f, WindowWidth, WindowHeight);
		m_pCost->setColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
	}

	//速度表示用
	{
		auto pObj = new GameObject(pMediator);
		pObj->setParent(&getUser());
		pObj->getTransform().setLocalPosition(Vec3(400.0f, -120.0f));
		m_pSpeed = pObj->addComponent<D2DTextRenderer>();
		m_pSpeed->setText(L"速度");
		m_pSpeed->setFont(L"Meiryo UI", L"ja-jp", fontSize);
		m_pSpeed->setFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_MEDIUM);
		m_pSpeed->setFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
		m_pSpeed->setFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
		m_pSpeed->setTextRect(0.0f, 0.0f, WindowWidth, WindowHeight);
		m_pSpeed->setColor(Color(1.0f, 1.0f, 1.0f, 1.0f));

		auto pSliderObj = new GameObject(pMediator);
		pSliderObj->getTransform().setLocalPosition(Vec3(500.0f, -120.0f, 1.0f));
		pSliderObj->getTransform().setLocalScale(Vec3(100.0f, 1.0f, 1.0f));
		pSliderObj->setParent(&getUser());
		m_pSpeedSlider = pSliderObj->addComponent<UISlider>();
		m_pSpeedSlider->setDirection(UISlider::Direction::RIGHT);
		m_pSpeedSlider->setWidth(16.0f);
		m_pSpeedSlider->setColor(Color(DirectX::Colors::Cyan));
	}
}
