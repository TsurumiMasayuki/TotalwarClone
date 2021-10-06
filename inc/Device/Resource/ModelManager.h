#pragma once
#include <unordered_map>
#include <Device\Resource\Model.h>

class ModelManager
{
public:
	void startLoading();
	void endLoading();

	void load(const std::string& key, const std::string& filePath, const std::string& modelName);
	void unloadAll();

	Model* getModel(const std::string& key);

private:
	std::unordered_map<std::string, Model*> m_Models;
};