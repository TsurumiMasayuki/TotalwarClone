#pragma once
#include "Component\Base\AbstractComponent.h"
#include "Effect\EffectRenderHelper.h"
#include "Utility\Timer.h"

class CubeTrailParticle
	: public AbstractComponent
{
public:
	virtual void onStart() override;
	virtual void onUpdate() override;

	void init(EffectRenderHelper* pEffectRenderHelper);

private:
	EffectRenderHelper* m_pEffectRenderHelper;
};

class CubeTrailEffect
	: public AbstractComponent
{
public:
	virtual void onStart() override;
	virtual void onUpdate() override;

	void init(EffectRenderHelper* pEffectRenderHelper);

private:
	Timer m_Timer;
	EffectRenderHelper* m_pEffectRenderHelper;
};