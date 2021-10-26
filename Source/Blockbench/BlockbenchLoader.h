#pragma once
#include <DirectXMath.h>
#include <string>
#include <unordered_map>
#include "json.hpp"
#include "Blockbench\BlockbenchModel.h"

//Blockbenchのモデル読み込み用クラス
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

	//面
	struct Face
	{
		//左上のuv
		UV uvUL;
		//右上のuv
		UV uvUR;
		//右下のuv
		UV uvDR;
		//左下のuv
		UV uvDL;

		//左上のUVから他のUVを計算
		Face(const UV& uvUL, float width, float height)
			: uvUL(uvUL),
			uvUR({ uvUL.x + width	, uvUL.y }),
			uvDR({ uvUL.x + width	, uvUL.y + height }),
			uvDL({ uvUL.x			, uvUL.y + height })
		{

		}
	};

public:
	void load(const std::string& filePath, const std::string& textureName, const std::string& key);
	void unLoadModels();
	const BlockbenchModel* getModel(const std::string& key) const;

private:
	bool safeGet(const nlohmann::json& source, nlohmann::json& result, const std::string& key) const;
	void multiplyParentMatrix(DirectX::XMMATRIX& result, const std::unordered_map<std::string, BoneInfo>& boneInfo, const std::string& parentName);

private:
	std::unordered_map<std::string, BlockbenchModel*> m_Models;
};