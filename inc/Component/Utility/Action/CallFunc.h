#pragma once
#include <functional>
#include <Component\Utility\Action\AbstractAction.h>

namespace Action
{
	class CallFunc
		: public AbstractAction
	{
	public:
		CallFunc(std::function<void()> function, float delay = 0.0f);

		virtual void init() override;
		virtual void update(float time) override;
		virtual void onSuspend() override;
		virtual AbstractAction * clone() override;

	private:
		std::function<void()> m_Function;
	};
}
