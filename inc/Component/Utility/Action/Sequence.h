#pragma once
#include "AbstractAction.h"
#include <deque>
#include <initializer_list>

namespace Action
{
	class Sequence
		: public AbstractAction
	{
	public:
		Sequence(std::deque<AbstractAction*> actionQueue);
		Sequence(std::initializer_list<AbstractAction*> actionList);
		~Sequence();

		virtual void init() override;
		virtual void update(float time) override;
		virtual void onSuspend() override;
		virtual bool isEnd() override;
		virtual void setUseUnscaledTime(bool value) override;

		virtual Action::Sequence* clone() override;

	private:
		std::deque<AbstractAction*> m_ActionQueue;
		std::deque<AbstractAction*> m_ActionQueueOrigin;
	};
}
