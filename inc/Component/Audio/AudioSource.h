#pragma once
#include <string>
#include <Component\Base\AbstractComponent.h>

class AudioInstance;

//音声再生用コンポーネント
class AudioSource
	: public AbstractComponent
{
public:
	AudioSource();
	~AudioSource();

	virtual void onStart() override;
	virtual void onUpdate() override;

	//再生(ループ回数が255の場合無限ループ。途中から再生した場合は無視されます。)
	void play(int loopCount = 0,
		float playBegin = 0.0f, float playEnd = 0.0f,
		float loopBegin = 0.0f, float loopEnd = 0.0f);
	
	//再生停止
	void stop();
	//ループ停止
	void stopLoop();
	//再生中断
	void pause();

	//音声を設定(ループ回数が255の場合無限ループ)
	void setAudio(const std::string& audioName, int loopCount = 0,
		float playBegin = 0.0f, float playEnd = 0.0f,
		float loopBegin = 0.0f, float loopEnd = 0.0f);

	//ボリューム設定(1.0fで通常)
	void setVolume(float volume);
	//ボリュームを取得
	float getVolume();

	//ピッチ設定(1.0fで通常)
	void setPitch(float pitch);
	//ピッチを取得
	float getPitch();

	//再生中かどうか
	bool isPlaying();

private:
	float m_Volume;
	std::string m_AudioName;			//音声アセット名
	AudioInstance* m_pAudioInstance;	//音声再生補助
};