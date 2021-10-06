#pragma once
#include <string>
#include <memory>

namespace DirectX
{
	struct WAVData;
}

//�����f�[�^�ێ��p
class AudioData
{
public:
	AudioData();
	~AudioData();

	//������(�ǂݍ���)
	void init(const std::wstring filePath);
	//WAVData�̎擾
	const DirectX::WAVData& getWAVData() const;
	
private:
	DirectX::WAVData* m_pWAVData;					//�Đ��p�f�[�^
	std::unique_ptr<uint8_t[]> m_pFileDataArray;	//�����f�[�^(�{��)
};