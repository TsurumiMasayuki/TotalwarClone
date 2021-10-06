#pragma once
#include <Component\Utility\Action\AbstractAction.h>

class AudioSource;

namespace Action
{
	class AudioCrossFade
		: public AbstractAction
	{
	public:
		AudioCrossFade(AudioSource* pOutAudio, AudioSource* pInAudio, float destVolume, float time);

		virtual void init() override;
		virtual void update(float time) override;
		virtual void onSuspend() override;
		virtual AbstractAction * clone() override;

	private:
		AudioSource* m_pOutAudio;
		AudioSource* m_pInAudio;

		float m_OriginVolume;
		float m_DestVolume;
	};
}