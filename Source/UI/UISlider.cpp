#include "UISlider.h"
#include "Actor\Base\GameObject.h"
#include "Component\Graphics\GUI\GUISpriteRenderer.h"

void UISlider::onStart()
{
	tryInitRenderer();
}

void UISlider::onUpdate()
{
	float ratio = m_CurrentValue / m_MaxValue;
	float position = 0.5f * (1.0f - ratio);
	auto& transform = m_pRendererObj->getTransform();

	//スライダーの方向に合わせてサイズ更新
	Vec3 size(0.0f, 0.0f, 1.0f);
	switch (m_SliderDirection)
	{
	case UISlider::Direction::RIGHT:
		transform.setLocalPosition(Vec3(-position, 0.0f, 0.0f));
		size.x = ratio;
		size.y = m_SliderWidth;
		break;
	case UISlider::Direction::LEFT:
		transform.setLocalPosition(Vec3(position, 0.0f, 0.0f));
		size.x = -ratio;
		size.y = m_SliderWidth;
		break;
	case UISlider::Direction::UP:
		transform.setLocalPosition(Vec3(0.0f, -position, 0.0f));
		size.x = m_SliderWidth;
		size.y = ratio;
		break;
	case UISlider::Direction::DOWN:
		transform.setLocalPosition(Vec3(0.0f, position, 0.0f));
		size.x = m_SliderWidth;
		size.y = -ratio;
		break;
	}

	//サイズ更新
	m_pRendererObj->getTransform().setLocalScale(size);
}

void UISlider::setCurrentValue(float currentValue)
{
	m_CurrentValue = currentValue;
}

float UISlider::getCurrentValue() const
{
	return m_CurrentValue;
}

void UISlider::setMaxValue(float maxValue)
{
	//0除算予防
	if (maxValue == 0.0f)
		assert(!"無効な最大値");
	m_MaxValue = maxValue;
}

float UISlider::getMaxValue() const
{
	return m_MaxValue;
}

void UISlider::setWidth(float width)
{
	m_SliderWidth = width;
}

float UISlider::getWidth() const
{
	return m_SliderWidth;
}

void UISlider::setTextureByName(const std::string& textureName)
{
	tryInitRenderer();

	m_pRenderer->setTextureByName(textureName);
}

void UISlider::setDirection(const Direction& direction)
{
	m_SliderDirection = direction;
}

void UISlider::setColor(const Color& color)
{
	tryInitRenderer();

	m_pRenderer->setColor(color);
}

void UISlider::tryInitRenderer()
{
	//Rendererが既に生成されているならreturn
	if (m_pRenderer != nullptr) return;

	m_pRendererObj = new GameObject(getUser().getGameMediator());
	m_pRendererObj->setParent(&getUser());
	m_pRenderer = m_pRendererObj->addComponent<GUISpriteRenderer>();
}
