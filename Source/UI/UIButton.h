#pragma once
#include <array>
#include <functional>
#include "Component\Base\AbstractComponent.h"

//ボタンクラス
class UIButton
	: public AbstractComponent
{
public:
	enum class MouseButtons
	{
		Left = 0,
		Right = 1,
		Middle = 2
	};

	virtual void onStart() override;
	virtual void onUpdate() override;

	//マウスクリックが離された時瞬間の関数を設定
	void setOnMouseButtonUp(const MouseButtons& mouseButton, const std::function<void()>& function);
	//マウスクリックした瞬間の関数を設定
	void setOnMouseButtonDown(const MouseButtons& mouseButton, const std::function<void()>& function);
	//マウスクリックしている間の関数を設定
	void setOnMouseButton(const MouseButtons& mouseButton, const std::function<void()>& function);

private:
	//マウスが上に乗っているか判定
	bool isMouseOn();

private:
	std::array<std::array<std::function<void()>, 3>, 3> m_MouseButtonFunctions;
};