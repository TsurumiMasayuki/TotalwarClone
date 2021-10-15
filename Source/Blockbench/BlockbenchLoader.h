#pragma once
#include <DirectXMath.h>
#include <string>
#include <unordered_map>
#include "json.hpp"

class BlockbenchModel;

class BlockbenchLoader
{
private:
	//ボーン情報保存用構造体
	struct BoneInfo
	{
		std::string parentName;
		DirectX::XMMATRIX boneMat;

		BoneInfo(const std::string& parentName, const DirectX::XMMATRIX& boneMat)
			: parentName(parentName),
			boneMat(boneMat)
		{

		}
	};

public:
	void loadModel(const std::string& filePath, const std::string& key);
	void unLoadModels();
	const BlockbenchModel* getModel(const std::string& key) const;

private:
	bool safeGet(const nlohmann::json& source, nlohmann::json& result, const std::string& key) const;
	void multiplyParentMatrix(DirectX::XMMATRIX& result, const std::unordered_map<std::string, BoneInfo>& boneInfo, const std::string& parentName);

private:
	std::unordered_map<std::string, BlockbenchModel*> m_Models;
};