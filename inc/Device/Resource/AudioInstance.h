#pragma once
class AudioData;
struct IXAudio2;
struct IXAudio2SourceVoice;

//SourceVoice�Đ��⏕
class AudioInstance
{
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="pXAudio2"></param>
	/// <param name="data"></param>
	/// <param name="loopCount">255�Ȃ疳�����[�v</param>
	/// <param name="playBegin">0�Ȃ�ŏ�����Đ�</param>
	/// <param name="playEnd">0�Ȃ�Ō�܂ōĐ�</param>
	/// <param name="loopBegin">0�Ȃ�ŏ����烋�[�v</param>
	/// <param name="loopEnd">0�Ȃ�Ō�܂Ń��[�v</param>
	AudioInstance(
		IXAudio2* pXAudio2,
		const AudioData& data,
		unsigned int loopCount = 0,
		float playBegin = 0.0f, float playEnd = 0.0f,
		float loopBegin = 0.0f, float loopEnd = 0.0f);
	~AudioInstance();

	//�Đ�
	void start();
	//�Đ���~
	void stop();
	//�Đ����~
	void pause();
	//���[�v�𒆎~(�Ō�܂ōĐ�����܂�)
	void stopLoop();

	//�Đ��I�����Ă��邩�ǂ���
	bool isEnd();

	//�{�����[���ݒ�(�{��)
	void setVolume(float volume);
	//�{�����[�����擾
	float getVolume();

	//�s�b�`�ݒ�(1.0�Ńf�t�H���g)
	void setPitch(float pitch);
	//�s�b�`���擾
	float getPitch();

private:
	IXAudio2SourceVoice* m_pSourceVoice;
	float m_CurrentVolume;
};