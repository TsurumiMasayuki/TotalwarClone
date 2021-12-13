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
	//�_�u���N���b�N�̗P�\���ԃ^�C�}�[�X�V
	for (int i = 0; i < 3; i++)
	{
		m_DoubleClickTimers.at(i).update();
	}

	//�}�E�X����ɖ����Ȃ�return
	if (!isMouseOn()) return;

	const auto& input = GameDevice::getInput();

	//���A�E�A���N���b�N�Ŕ���A���s
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

	//�_�u���N���b�N�̔���(����������A�E�A��)
	for (int i = 0; i < 3; i++)
	{
		//�N���b�N��
		if (input.isMouseButtonDown(i))
		{
		 	auto& timer = m_DoubleClickTimers.at(i);

			//�P�\���ԓ��ł����
			if (!timer.isTime())
			{
				//�_�u���N���b�N���̊֐������s
				m_DoubleClickFunctions.at(i)();
			}

			//�^�C�}�[�����Z�b�g
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

	//���g�̍��W��ɂ���
	float halfWidth = size.x * 0.5f;
	float halfHeight = size.y * 0.5f;

	//�͈͓�������
	return MathUtility::isInRange(mousePosition.x, -halfWidth + position.x, halfWidth + position.x) &&
		MathUtility::isInRange(mousePosition.y, -halfHeight + position.y, halfHeight + position.y);
}
