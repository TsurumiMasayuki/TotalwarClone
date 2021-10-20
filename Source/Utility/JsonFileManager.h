#pragma once
#include <string>
#include <unordered_map>
#include <fstream>
#include "json.hpp"

#include "Unit\Attack\AttackStats.h"
#include "Utility\SingletonTemplate.h"

//Json�Ǘ��N���X
template<typename T>
class JsonFileManager
	: public SingletonTemplate<JsonFileManager<T>>
{
public:
	friend class SingletonTemplate<JsonFileManager<T>>;

	//�t�@�C���̓ǂݍ���
	void load(const std::string& key, const std::string& filePath);
	//���
	void unloadAll();
	//�X�e�[�^�X�̎擾
	const T& get(const std::string& key) const;

protected:
	//�R���X�g���N�^���B��
	JsonFileManager() {};
	//�f�X�g���N�^���B��
	virtual ~JsonFileManager() {};

private:
	std::unordered_map<std::string, T> m_FileList;
};

template<typename T>
inline void JsonFileManager<T>::load(const std::string& key, const std::string& filePath)
{
	//�t�@�C���ǂݍ���
	std::ifstream ifs(filePath);

	//json�֕ϊ�
	nlohmann::json json;
	ifs >> json;
	ifs.close();

	//�X�e�[�^�X�ǂݍ���
	T file(json);

	//�U���N���X��o�^
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