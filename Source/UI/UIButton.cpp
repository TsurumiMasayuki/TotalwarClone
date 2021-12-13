#include "UIButton.h"
#include "Math\MathUtility.h"
#include "Component\Utility\Transform.h"
#include "Device\GameDevice.h"

void UIButton::onStart()
{
	for (int i = 0; i < 3; i++)
	{
		m_DoubleClickTimers.at(i).setMaxTime(0.5f);
	}
}

void UIButton::onUpdate()
{
	//ダブルクリックの猶予時間タイマー更新
	for (int i = 0; i < 3; i++)
	{
		m_DoubleClickTimers.at(i).update();
	}

	//マウスが上に無いならreturn
	if (!isMouseOn()) return;

	const auto& input = GameDevice::getInput();

	//左、右、中クリックで判定、実行
	for (int i = 0; i < 3; i++)
	{
		if (input.isMouseButtonUp(i))
		{
			if (m_MouseButtonFunctions.at(0).at(i))
				m_MouseButtonFunctions.at(0).at(i)();
		}

		if (input.isMouseButtonDown(i))
		{
			if (m_MouseButtonFunctions.at(1).at(i))
				m_MouseButtonFunctions.at(1).at(i)();
		}

		if (input.isMouseButton(i))
		{
			if (m_MouseButtonFunctions.at(2).at(i))
				m_MouseButtonFunctions.at(2).at(i)();
		}
	}

	//ダブルクリックの判定(こちらも左、右、中)
	for (int i = 0; i < 3; i++)
	{
		//クリック時
		if (input.isMouseButtonDown(i))
		{
		 	auto& timer = m_DoubleClickTimers.at(i);

			//猶予時間内であれば
			if (!timer.isTime())
			{
				//ダブルクリック時の関数を実行
				m_DoubleClickFunctions.at(i)();
			}

			//タイマーをリセット
			timer.reset();
		}
	}
}

void UIButton::setOnMouseButtonUp(const MouseButtons& mouseButton, const std::function<void()>& function)
{
	m_MouseButtonFunctions.at(0).at((int)mouseButton) = function;
}

void UIButton::setOnMouseButtonDown(const MouseButtons& mouseButton, const std::function<void()>& function)
{
	m_MouseButtonFunctions.at(1).at((int)mouseButton) = function;
}

void UIButton::setOnMouseButton(const MouseButtons& mouseButton, const std::function<void()>& function)
{
	m_MouseButtonFunctions.at(2).at((int)mouseButton) = function;
}

void UIButton::setOnMouseButtonDoubleClick(const MouseButtons& mouseButton, const std::function<void()>& function)
{
	m_DoubleClickFunctions.at((int)mouseButton) = function;
}

bool UIButton::isMouseOn()
{
	const Vec3& position = getTransform().getLocalPosition();
	const Vec3& size = getTransform().getLocalScale();
	const Vec3 mousePosition = GameDevice::getInput().getMousePosition();

	//自身の座標基準にする
	float halfWidth = size.x * 0.5f;
	float halfHeight = size.y * 0.5f;

	//範囲内か判定
	return MathUtility::isInRange(mousePosition.x, -halfWidth + position.x, halfWidth + position.x) &&
		MathUtility::isInRange(mousePosition.y, -halfHeight + position.y, halfHeight + position.y);
}
