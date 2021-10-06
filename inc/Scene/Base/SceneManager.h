#pragma once
#include <string>
#include <unordered_map>
#include <Utility\INonCopyable.h>

class AbstractScene;

//�V�[���Ǘ��N���X
class SceneManager
	: private INonCopyable
{
public:
	SceneManager();
	~SceneManager();

	//�X�V����
	void update();
	//�����I��
	void shutdown();

	//�V�[���Ǘ��ɃV�[���ǉ�
	void addScene(std::string key, AbstractScene* pAbstractScene);
	//�V�[���ύX
	void changeScene(std::string key);

private:
	//���݂̃V�[����
	std::string m_CurrentScene;
	//�V�[���Ǘ��pmap
	std::unordered_map<std::string, AbstractScene*> m_Scenes;
};

