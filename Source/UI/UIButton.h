#pragma once
#include <array>
#include <functional>
#include "Component\Base\AbstractComponent.h"

//�{�^���N���X
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

	//�}�E�X�N���b�N�������ꂽ���u�Ԃ̊֐���ݒ�
	void setOnMouseButtonUp(const MouseButtons& mouseButton, const std::function<void()>& function);
	//�}�E�X�N���b�N�����u�Ԃ̊֐���ݒ�
	void setOnMouseButtonDown(const MouseButtons& mouseButton, const std::function<void()>& function);
	//�}�E�X�N���b�N���Ă���Ԃ̊֐���ݒ�
	void setOnMouseButton(const MouseButtons& mouseButton, const std::function<void()>& function);

private:
	//�}�E�X����ɏ���Ă��邩����
	bool isMouseOn();

private:
	std::array<std::array<std::function<void()>, 3>, 3> m_MouseButtonFunctions;
};