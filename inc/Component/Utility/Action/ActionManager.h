#pragma once
#include <queue>
#include <Component\Base\AbstractComponent.h>

class GameObject;

namespace Action
{
	class AbstractAction;

	class ActionManager
		: public AbstractComponent
	{
	public:
		ActionManager() : m_UseUnscaledTime(false) {}
		~ActionManager();

		virtual void onStart() override;
		virtual void onUpdate() override;

		void enqueueAction(AbstractAction* action);
		void forceNext();
		int actionCount();
		void setUseUnscaledTime(bool useUnscaledTime) { m_UseUnscaledTime = useUnscaledTime; }

	private:
		void actionChange();

	private:
		std::queue<AbstractAction*> m_ActionQueue;
		bool m_UseUnscaledTime;
	};
}