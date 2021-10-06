#pragma once
class AudioData;
struct IXAudio2;
struct IXAudio2SourceVoice;

//SourceVoice再生補助
class AudioInstance
{
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="pXAudio2"></param>
	/// <param name="data"></param>
	/// <param name="loopCount">255なら無限ループ</param>
	/// <param name="playBegin">0なら最初から再生</param>
	/// <param name="playEnd">0なら最後まで再生</param>
	/// <param name="loopBegin">0なら最初からループ</param>
	/// <param name="loopEnd">0なら最後までループ</param>
	AudioInstance(
		IXAudio2* pXAudio2,
		const AudioData& data,
		unsigned int loopCount = 0,
		float playBegin = 0.0f, float playEnd = 0.0f,
		float loopBegin = 0.0f, float loopEnd = 0.0f);
	~AudioInstance();

	//再生
	void start();
	//再生停止
	void stop();
	//再生中止
	void pause();
	//ループを中止(最後まで再生されます)
	void stopLoop();

	//再生終了しているかどうか
	bool isEnd();

	//ボリューム設定(倍率)
	void setVolume(float volume);
	//ボリュームを取得
	float getVolume();

	//ピッチ設定(1.0でデフォルト)
	void setPitch(float pitch);
	//ピッチを取得
	float getPitch();

private:
	IXAudio2SourceVoice* m_pSourceVoice;
	float m_CurrentVolume;
};