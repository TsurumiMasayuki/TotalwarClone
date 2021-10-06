#pragma once
#include <string>
#include <Component\Base\AbstractComponent.h>

class AudioInstance;

//�����Đ��p�R���|�[�l���g
class AudioSource
	: public AbstractComponent
{
public:
	AudioSource();
	~AudioSource();

	virtual void onStart() override;
	virtual void onUpdate() override;

	//�Đ�(���[�v�񐔂�255�̏ꍇ�������[�v�B�r������Đ������ꍇ�͖�������܂��B)
	void play(int loopCount = 0,
		float playBegin = 0.0f, float playEnd = 0.0f,
		float loopBegin = 0.0f, float loopEnd = 0.0f);
	
	//�Đ���~
	void stop();
	//���[�v��~
	void stopLoop();
	//�Đ����f
	void pause();

	//������ݒ�(���[�v�񐔂�255�̏ꍇ�������[�v)
	void setAudio(const std::string& audioName, int loopCount = 0,
		float playBegin = 0.0f, float playEnd = 0.0f,
		float loopBegin = 0.0f, float loopEnd = 0.0f);

	//�{�����[���ݒ�(1.0f�Œʏ�)
	void setVolume(float volume);
	//�{�����[�����擾
	float getVolume();

	//�s�b�`�ݒ�(1.0f�Œʏ�)
	void setPitch(float pitch);
	//�s�b�`���擾
	float getPitch();

	//�Đ������ǂ���
	bool isPlaying();

private:
	float m_Volume;
	std::string m_AudioName;			//�����A�Z�b�g��
	AudioInstance* m_pAudioInstance;	//�����Đ��⏕
};