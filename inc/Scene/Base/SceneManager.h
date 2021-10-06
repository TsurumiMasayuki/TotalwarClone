#pragma once
#include <string>
#include <unordered_map>
#include <Utility\INonCopyable.h>

class AbstractScene;

//シーン管理クラス
class SceneManager
	: private INonCopyable
{
public:
	SceneManager();
	~SceneManager();

	//更新処理
	void update();
	//強制終了
	void shutdown();

	//シーン管理にシーン追加
	void addScene(std::string key, AbstractScene* pAbstractScene);
	//シーン変更
	void changeScene(std::string key);

private:
	//現在のシーン名
	std::string m_CurrentScene;
	//シーン管理用map
	std::unordered_map<std::string, AbstractScene*> m_Scenes;
};

