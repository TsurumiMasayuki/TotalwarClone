#pragma once
#include <functional>
#include "Component\Base\AbstractComponent.h"
#include "UI\UISlider.h"

template<typename T>
class UISliderHelper
	: public AbstractComponent
{
public:
	virtual void onStart() override;
	virtual void onUpdate() override;

	void init(UISlider* pTargetSlider, T* pSource, const std::function<float(T*)>& function);

private:
	UISlider* m_pTargetSlider;
	T* m_pSourceObject;
	std::function<float(T*)> m_Function;
};

template<typename T>
inline void UISliderHelper<T>::onStart()
{
}

template<typename T>
inline void UISliderHelper<T>::onUpdate()
{
	m_pTargetSlider->setCurrentValue(m_Function(m_pSourceObject));
}

template<typename T>
inline void UISliderHelper<T>::init(UISlider* pTargetSlider, T* pSource, const std::function<float(T*)>& function)
{
	m_pTargetSlider = pTargetSlider;
	m_pSourceObject = pSource;
	m_Function = function;
}
