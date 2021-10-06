#pragma once
#include <string>
#include <unordered_map>
#include <wrl.h>
#include <d3d12.h>

#include <Graphics\DX12\DX12Texture2D.h>

//TODO 
//�I�[�g���[�h�̎���
//�A�����[�h�̎���(���p�҂ɒʒm������F�X���Ȃ��Ƃ����Ȃ�)

class TextureManager
{
public:
	void init();

	//�e�N�X�`���̃��[�h
	void load(const std::string& key, const std::wstring& filePath);
	//�S�ăA�����[�h
	void unloadAll();

	//�e�N�X�`���̎擾
	DX12Texture2D* getTexture(const std::string& key) const;

private:
	std::unordered_map<std::string, DX12Texture2D*> m_Textures;
};