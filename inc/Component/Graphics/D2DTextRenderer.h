#pragma once
#include <d2d1_3.h>
#include <dwrite.h>
#include <wrl.h>
#include <string>

#include "Component\Base\AbstractComponent.h"
#include "Component\Graphics\ID2DRenderer.h"
#include "Utility\Color.h"

//Direct2Dを用いたテキストレンダラー
class D2DTextRenderer
	: public AbstractComponent,
	public ID2DRenderer
{
public:
	D2DTextRenderer();
	~D2DTextRenderer();

	virtual void onStart() override;
	virtual void onUpdate() override;

	void setText(const std::wstring& text);
	const std::wstring& getText() const;

	void setTextRect(float centerX, float centerY, float width, float height);

	void setColor(const Color& textColor);
	const Color& getColor() const;

	void setFont(const std::wstring& fontName, const std::wstring& localeName, float fontSize);

	void setFontWeight(const DWRITE_FONT_WEIGHT& fontWeight);
	void setFontStyle(const DWRITE_FONT_STYLE& fontStyle);
	void setFontStretch(const DWRITE_FONT_STRETCH& fontStretch);

	void setTextAlignment(const DWRITE_TEXT_ALIGNMENT& alignment);
	void setParagraphAlignment(const DWRITE_PARAGRAPH_ALIGNMENT& alignment);

	// ID2DRenderer を介して継承されました
	virtual void draw(ID2D1DeviceContext2* pDeviceContext) override;

private:
	void updateColorBrush();
	void updateTextFormat();

private:
	bool m_IsBrushDirty;
	bool m_IsTextFormatDirty;

	std::wstring m_Text;
	Color m_TextColor;

	D2D_RECT_F m_TextRect;

	std::wstring m_FontName;
	std::wstring m_LocaleName;
	float m_FontSize;
	DWRITE_FONT_WEIGHT m_FontWeight;
	DWRITE_FONT_STYLE m_FontStyle;
	DWRITE_FONT_STRETCH m_FontStretch;

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_pColorBrush;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pTextFormat;
};