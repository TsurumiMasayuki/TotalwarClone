#pragma once
#include "AbstractAction.h"

class AudioSource;

namespace Action
{
	class PlayAudio
		: public AbstractAction
	{
	public:
		PlayAudio(AudioSource* pAudioSource);
		~PlayAudio();

		virtual void init() override;
		virtual void update(float time) override;
		virtual void onSuspend() override;

		virtual bool isEnd() override { return true; }
		virtual Action::PlayAudio* clone() override;

	private:
		AudioSource* m_pAudioSource;
	};
};