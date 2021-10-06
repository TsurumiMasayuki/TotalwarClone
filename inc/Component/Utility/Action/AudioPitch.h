#pragma once
#include <Component\Utility\Action\AbstractAction.h>

class AudioSource;

namespace Action
{
	class AudioPitch
		: public AbstractAction
	{
	public:
		AudioPitch(AudioSource* pAudioSource, float destPitch, float time);

		virtual void init() override;
		virtual void update(float time) override;
		virtual void onSuspend() override;
		virtual AbstractAction * clone() override;

	private:
		AudioSource* m_pAudioSource;
		float m_OriginPitch;
		float m_DestPitch;
	};
}