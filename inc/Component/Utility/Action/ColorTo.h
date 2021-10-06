#pragma once
#include "AbstractAction.h"
#include <Component\Graphics\IRenderer.h>
#include <Utility\Color.h>

namespace Action
{
	class ColorTo
		: public AbstractAction
	{
	public:
		ColorTo(const Color& destColor, IRenderer* pRenderer, float time);
		~ColorTo();

		virtual void init() override;
		virtual void update(float time) override;
		virtual void onSuspend() override;

		virtual Action::ColorTo* clone() override;

	private:
		IRenderer* m_pRenderer;

		Color m_OriginColor;
		Color m_DestColor;
	};
};