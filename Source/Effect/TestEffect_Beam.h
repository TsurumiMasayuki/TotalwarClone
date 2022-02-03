#pragma once
#include "Component\Base\AbstractComponent.h"
#include "Math\MathUtility.h"

#include "Effect\IEffect.h"
#include "Effect\EffectRenderHelper.h"

class AudioSource;
class Transform;

namespace Action
{
	class ActionManager;
}

class TestEffect_Beam
	: public AbstractComponent,
	public IEffect
{
public:
	virtual void onStart() override;
	virtual void onUpdate() override;

	void setBeginPos(const Vec3& position);
	void setEndPos(const Vec3& position);

	void setWidth(float width);
	void setTime(float time);

	void init(EffectRenderHelper* pEffectRenderHelper, const Color& color);
	virtual void playEffect() override;

private:
	Vec3 m_BeginPos;
	Vec3 m_EndPos;

	float m_Width;
	float m_Time;

	AudioSource* m_pSEAudio;
	Transform* m_pBeamTransform;
	Action::ActionManager* m_pActionManager;
	EffectRenderHelper* m_pEffectRenderHelper;
	Color m_Color;
};