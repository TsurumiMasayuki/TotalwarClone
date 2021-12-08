#pragma once
#include <string>
#include "Component\Base\AbstractComponent.h"
#include "Utility\Color.h"

class GUISpriteRenderer;

//�X���C�_�[�N���X�B�X���C�_�[�̃I�u�W�F�N�g�̃T�C�Y�𒲐�
class UISlider
	: public AbstractComponent
{
public:
	//�X���C�_�[�̕���
	enum class Direction
	{
		RIGHT,
		LEFT,
		UP,
		DOWN
	};

public:
	virtual void onStart() override;
	virtual void onUpdate() override;

	void setCurrentValue(float currentValue);
	float getCurrentValue() const;

	void setMaxValue(float maxValue);
	float getMaxValue() const;

	void setWidth(float width);
	float getWidth() const;

	void setTextureByName(const std::string& textureName);
	void setDirection(const Direction& direction);
	void setColor(const Color& color);

private:
	void tryInitRenderer();

private:
	float m_CurrentValue;
	float m_MaxValue;

	GameObject* m_pRendererObj;
	GUISpriteRenderer* m_pRenderer;

	GameObject* m_pBackGroundObj;

	float m_SliderWidth;
	Direction m_SliderDirection;
};