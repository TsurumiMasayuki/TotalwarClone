#pragma once
#include "AbstractAction.h"

namespace Action
{
	class Destroy
		: public AbstractAction
	{
	public:
		Destroy(float time);
		~Destroy();

		virtual void init() override;
		virtual void update(float time) override;
		virtual void onSuspend() override;

		virtual Action::Destroy* clone() override;
	};
};