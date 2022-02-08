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
};

template<typename T>
inline void JsonFileManager<T>::load(const std::string& key, const std::string& filePath)
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
	m_FileList.emplace(key, file);
}

template<typename T>
inline void JsonFileManager<T>::unloadAll()
{
	m_FileList.clear();
}

template<typename T>
inline const T& JsonFileManager<T>::get(const std::string& key) const
{
	return m_FileList.at(key);
}

template<typename T>
inline const std::unordered_map<std::string, T>& JsonFileManager<T>::getAll() const
{
	return m_FileList;
}
