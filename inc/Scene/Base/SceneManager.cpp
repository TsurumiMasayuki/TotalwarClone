#include "SceneManager.h"
#include <Scene\Base\AbstractScene.h>

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
	for (auto scene : m_Scenes)
	{
		delete scene.second;
	}
	m_Scenes.clear();
}

void SceneManager::update()
{
	if (m_CurrentScene.empty())
		return;

	m_Scenes.at(m_CurrentScene)->baseUpdate();
	m_Scenes.at(m_CurrentScene)->lateUpdate();
	m_Scenes.at(m_CurrentScene)->draw();

	if (m_Scenes.at(m_CurrentScene)->isEnd())
		changeScene(m_Scenes.at(m_CurrentScene)->nextScene());
}

void SceneManager::shutdown()
{
	m_Scenes.at(m_CurrentScene)->baseShutdown();
}

void SceneManager::addScene(std::string key, AbstractScene * pAbstractScene)
{
	m_Scenes.emplace(key, pAbstractScene);
}

void SceneManager::changeScene(std::string key)
{
	if (!m_CurrentScene.empty())
	{
		//�Â��V�[���̏I������
		m_Scenes.at(m_CurrentScene)->baseShutdown();
	}

	//���݂̃V�[�������X�V
	m_CurrentScene = key;

	//���̊J�n����
	m_Scenes.at(m_CurrentScene)->baseStart();
}
