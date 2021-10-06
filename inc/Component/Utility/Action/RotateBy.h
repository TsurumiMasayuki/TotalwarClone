#pragma once
#include "AbstractAction.h"
#include "Math\Vec3.h"

namespace Action
{
	class RotateBy
		: public AbstractAction
	{
	public:
		RotateBy(Vec3 rotateValue, float time);
		~RotateBy();

		virtual void init() override;
		virtual void update(float time) override;
		virtual void onSuspend() override;

		virtual Action::RotateBy* clone() override;

	private:
		Vec3 m_Origin;
		Vec3 m_RotateValue;
		Vec3 m_DestAngles;
	};
};