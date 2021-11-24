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
		//古いシーンの終了処理
		m_Scenes.at(m_CurrentScene)->baseShutdown();
	}

	//現在のシーン名を更新
	m_CurrentScene = key;

	//次の開始処理
	m_Scenes.at(m_CurrentScene)->baseStart();
}
