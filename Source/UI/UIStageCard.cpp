#include "UIStageCard.h"

#include "Actor\Base\GameObject.h"
#include "Component\Graphics\D2DTextRenderer.h"
#include "Component\Graphics\GUI\GUISpriteRenderer.h"
#include "Device\GameDevice.h"
#include "Utility\StringUtility.h"

#include "Stage\Stage.h"
#include "Utility\JsonFileManager.h"

void UIStageCard::onStart()
{
}

void UIStageCard::onUpdate()
{
}

void UIStageCard::onLocalPositionChanged(const Vec3& newPosition)
{
	m_pStageNameRenderer->setTextRect(newPosition.x, newPosition.y, newPosition.x + 1280.0f, newPosition.y + 720.0f);
}

void UIStageCard::setStage(const std::string& stageName)
{
	const auto& stage = JsonFileManager<Stage>::getInstance().get(stageName);

	if (m_pStageNameRenderer == nullptr)
	{
		auto pSpriteObj = new GameObject(getUser().getGameMediator());
		pSpriteObj->setParent(&getUser());
		//SpriteRendererの設定
		GUISpriteRenderer* pSpriteRenderer = pSpriteObj->addComponent<GUISpriteRenderer>();
		pSpriteRenderer->setTexture(GameDevice::getTextureManager().getTexture("BoxFill"));

		auto pTextObj = new GameObject(getUser().getGameMediator());
		pTextObj->setParent(&getUser());
		const Vec3 basePos = getUser().getTransform().getLocalPosition();

		//TextRendererの設定
		m_pStageNameRenderer = pTextObj->addComponent<D2DTextRenderer>();
		m_pStageNameRenderer->setFont(L"Meiryo UI", L"ja-jp", 48.0f);
		m_pStageNameRenderer->setFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_MEDIUM);
		m_pStageNameRenderer->setFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
		m_pStageNameRenderer->setFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
		m_pStageNameRenderer->setColor(Color(0.0f, 0.0f, 0.0f, 1.0f));
		m_pStageNameRenderer->setTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
		m_pStageNameRenderer->setParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		m_pStageNameRenderer->setTextRect(basePos.x, basePos.y, basePos.x + 1280.0f, basePos.y + 720.0f);
	}

	//ステージ名を描画
	m_pStageNameRenderer->setText(StringUtility::ToWString(stage.m_Name));
}