#pragma once
#include "AbstractAction.h"
#include "Math\Vec3.h"

namespace Action
{
	class RotateTo
		: public AbstractAction
	{
	public:
		RotateTo(Vec3 destAngles, float time);
		~RotateTo();

		virtual void init() override;
		virtual void update(float time) override;
		virtual void onSuspend() override;
		virtual Action::RotateTo* clone() override;

	private:
		Vec3 m_Origin;
		Vec3 m_DestAngles;
	};
};