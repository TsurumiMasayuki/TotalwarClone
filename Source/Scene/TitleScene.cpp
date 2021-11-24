#include "TitleScene.h"
#include "Actor\Base\GameObject.h"
#include "Component\Graphics\D2DTextRenderer.h"
#include "Device\GameDevice.h"

std::string TitleScene::nextScene()
{
	return "SelectScene";
}

bool TitleScene::isEnd()
{
	return GameDevice::getInput().isKeyDown(DIK_SPACE);
}

void TitleScene::start()
{
	GameObject* pObj = new GameObject(this);
	auto pTextRenderer = pObj->addComponent<D2DTextRenderer>();
	pTextRenderer->setFont(L"Meiryo UI", L"ja-jp", 128.0f);
	pTextRenderer->setFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_MEDIUM);
	pTextRenderer->setFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
	pTextRenderer->setFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
	pTextRenderer->setColor(Color(0.0f, 0.0f, 0.0f, 1.0f));
	pTextRenderer->setTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
	pTextRenderer->setParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	pTextRenderer->setTextRect(0.0f, 0.0f, 1280.0f, 720.0f);
	pTextRenderer->setText(L"ƒ^ƒCƒgƒ‹");
}

void TitleScene::update()
{
}

void TitleScene::shutdown()
{
}
