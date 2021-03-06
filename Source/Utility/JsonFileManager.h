#pragma once
#include <string>
#include <unordered_map>
#include <fstream>
#include "json.hpp"

#include "Unit\Attack\AttackStats.h"
#include "Utility\SingletonTemplate.h"

//Json管理クラス
template<typename T>
class JsonFileManager
	: public SingletonTemplate<JsonFileManager<T>>
{
public:
	friend class SingletonTemplate<JsonFileManager<T>>;

	//ファイルの読み込み
	void load(const std::string& key, const std::string& filePath);
	//ファイルの再読み込み
	void reloadAll();
	//解放
	void unloadAll();
	//取得
	const T& get(const std::string& key) const;
	//全て取得
	const std::unordered_map<std::string, T>& getAll() const;

protected:
	//コンストラクタを隠す
	JsonFileManager() {};
	//デストラクタを隠す
	virtual ~JsonFileManager() {};

private:
	std::unordered_map<std::string, T> m_FileList;
	std::unordered_map<std::string, std::string> m_FilePathList;
};

template<typename T>
void JsonFileManager<T>::load(const std::string& key, const std::string& filePath)
{
	//ファイル読み込み
	std::ifstream ifs(filePath);

	//jsonへ変換
	nlohmann::json json;
	ifs >> json;
	ifs.close();

	//ステータス読み込み
	T file(json);

	//jsonファイルを登録
	if (m_FileList.contains(key))
		m_FileList.at(key) = file;
	else
		m_FileList.emplace(key, file);
	//ファイルパスを登録
	if (!m_FilePathList.contains(key))
		m_FilePathList.emplace(key, filePath);
}

template<typename T>
void JsonFileManager<T>::reloadAll()
{
	//ファイルパスリストを参照して全て読み込みなおす
	for (const auto& pair : m_FilePathList)
	{
		load(pair.first, pair.second);
	}
}

template<typename T>
void JsonFileManager<T>::unloadAll()
{
	m_FileList.clear();
	m_FilePathList.clear();
}

template<typename T>
const T& JsonFileManager<T>::get(const std::string& key) const
{
	return m_FileList.at(key);
}

template<typename T>
const std::unordered_map<std::string, T>& JsonFileManager<T>::getAll() const
{
	return m_FileList;
}
