#include "BlockbenchLoader.h"
#include <fstream>

#include "Device\GameDevice.h"
#include "Math\Vec3.h"
#include "Math\MathUtility.h"

using namespace DirectX;
using namespace nlohmann;

void BlockbenchLoader::load(const std::string& filePath, const std::string& textureName, const std::string& key)
{
	//ファイルを読み込み
	std::fstream fs(filePath);
	assert(fs.is_open());

	//読み込んだファイルをjsonに変換
	json jsonFile;
	fs >> jsonFile;

	//ボーン情報保存用map
	std::unordered_map<std::string, BoneInfo> boneInfo;

	//結果用vector
	std::vector<XMMATRIX> resultMatrices;
	std::vector<UV> resultUVOrigins;
	std::vector<UV> resultUVSizes;

	//モデルの情報を取得
	auto& description = jsonFile["minecraft:geometry"][0]["description"];
	//テクスチャの幅
	float textureWidth = description["texture_width"];
	//テクスチャの高さ
	float textureHeight = description["texture_height"];

	//ボーンリストを取得
	auto& bones = jsonFile["minecraft:geometry"][0]["bones"];

	//オフセット
	DirectX::XMMATRIX modelOffset = DirectX::XMMatrixTranslation(0.0f, -50.0f, 0.0f);

	//ボーンごとに処理
	for (auto& bone : bones)
	{
		//ボーン内の立方体リストを取得
		json cubes;
		if (!safeGet(bone, cubes, "cubes"))
			continue;

		//ボーンの情報を取得
		json boneName;
		boneName = bone["name"];

		//回転情報を読み取って行列に変換
		json boneRotation;
		XMMATRIX boneRotationMat = XMMatrixIdentity();
		if (safeGet(bone, boneRotation, "rotation"))
		{
			boneRotationMat = XMMatrixRotationRollPitchYaw(MathUtility::toRadian(boneRotation[0]),
				MathUtility::toRadian(boneRotation[1]),
				MathUtility::toRadian(boneRotation[2])
			);
		}

		//ピボット情報を読み取って行列に変換
		json bonePivot;
		XMMATRIX bonePivotMat = XMMatrixIdentity();
		if (safeGet(bone, bonePivot, "pivot"))
		{
			bonePivotMat = XMMatrixTranslation(-(float)(bonePivot[0] - 1.0f) * 0.5f, -(float)(bonePivot[1]), (float)(bonePivot[2]));
		}

		//行列を合成
		XMMATRIX boneMat = bonePivotMat * boneRotationMat;

		//親の名前を読み取って親の行列を合成する
		json parentName = "";
		XMMATRIX parentMatrix = boneMat;
		if (safeGet(bone, parentName, "parentName"))
		{
			multiplyParentMatrix(parentMatrix, boneInfo, parentName);
		}

		//ボーン行列を保存
		boneInfo.emplace(boneName, BoneInfo(parentName, boneMat));

		//立方体ごとに処理
		for (auto& cube : cubes)
		{
			//座標情報を読み取って行列に変換
			json origin;
			XMMATRIX translation = XMMatrixIdentity();
			if (safeGet(cube, origin, "origin"))
			{
				translation = XMMatrixTranslation(origin[0], origin[1], origin[2]);
			}

			//スケール情報を読み取って行列に変換
			json size;
			XMMATRIX scaling = XMMatrixIdentity();
			if (safeGet(cube, size, "size"))
			{
				scaling = XMMatrixScalingFromVector((Vec3(size[0], size[1], size[2]) * 0.05f).toXMVector());
			}

			//回転情報を読み取って行列に変換
			json rotation;
			XMMATRIX rotationMat = XMMatrixIdentity();
			if (safeGet(cube, rotation, "rotation"))
			{
				rotationMat = XMMatrixRotationRollPitchYaw(MathUtility::toRadian(rotation[0]),
					MathUtility::toRadian(rotation[1]),
					MathUtility::toRadian(rotation[2])
				);
			}

			//ピボット情報を読み取って行列に変換
			json pivot;
			XMMATRIX pivotMat = XMMatrixIdentity();
			if (safeGet(cube, pivot, "pivot"))
			{
				pivotMat = XMMatrixTranslation(pivot[0] - 0.5f, pivot[1], pivot[2]);
			}

			//スケールの1/2分ずらす
			XMMATRIX offsetMat = XMMatrixTranslation(size[0] / 2, size[1] / 2, size[2] / 2);

			//行列を全て合成
			XMMATRIX world = modelOffset * scaling * parentMatrix * rotationMat * pivotMat * translation * offsetMat;
			resultMatrices.emplace_back(world);

			//UVを読み込み
			float originX = (float)cube["uv"][0] / textureWidth;
			float originY = (float)cube["uv"][1] / textureHeight;

			float xSize = (float)size[0];
			float ySize = (float)size[1];
			float zSize = (float)size[2];

			Face faces[6] = 
			{
				//+Zの面
				Face(UV(originX + zSize / textureWidth, originY + zSize / textureHeight),
					xSize / textureWidth,
					ySize / textureHeight),
				//+Xの面
				Face(UV(originX, originY + zSize / textureHeight),
					zSize / textureWidth,
					ySize / textureHeight),
				//-Xの面
				Face(UV(originX + (xSize + zSize) / textureWidth, originY + zSize / textureHeight),
					zSize / textureWidth,
					ySize / textureHeight),
				//-Zの面
				Face(UV(originX + (xSize + zSize * 2) / textureWidth, originY + zSize / textureHeight),
					xSize / textureWidth,
					ySize / textureHeight),
				//+Yの面
				Face(UV(originX + zSize / textureWidth, originY),
					xSize / textureWidth,
					zSize / textureHeight),
				//-Yの面
				Face(UV(originX + (xSize + zSize) / textureWidth, originY),
					xSize / textureWidth,
					zSize / textureHeight)
			};

			//面ごとにループ
			for (int i = 0; i < BlockbenchModel::cubeFaceCount; i++)
			{
				resultUVOrigins.emplace_back();
				resultUVSizes.emplace_back();

				resultUVOrigins.back() = faces[i].uvUL;
				resultUVSizes.back() = UV(faces[i].uvDR.x - faces[i].uvUL.x, faces[i].uvDR.y - faces[i].uvUL.y);
			}
		}
	}

	//モデルを登録
	m_Models.emplace(key, new BlockbenchModel(resultMatrices, resultUVOrigins, resultUVSizes, textureName));
}

void BlockbenchLoader::unLoadModels()
{
	for (auto pModel : m_Models)
	{
		delete pModel.second;
	}

	m_Models.clear();
}

const BlockbenchModel* BlockbenchLoader::getModel(const std::string& key) const
{
	return m_Models.at(key);
}

bool BlockbenchLoader::safeGet(const nlohmann::json& source, nlohmann::json& result, const std::string& key) const
{
	//keyが含まれているかどうか
	bool contains = source.contains(key);

	//含まれていたらGet
	if (contains)
	{
		result = source[key];
	}

	return contains;
}

void BlockbenchLoader::multiplyParentMatrix(DirectX::XMMATRIX& result,
	const std::unordered_map<std::string, BlockbenchLoader::BoneInfo>& boneInfo,
	const std::string& parentName)
{
	//指定された親ボーンの名前が無いならreturn
	if (!boneInfo.contains(parentName)) return;

	auto& parent = boneInfo.at(parentName);

	//親の行列を掛ける
	result *= parent.boneMat;
	//再帰呼び出し
	multiplyParentMatrix(result, boneInfo, parent.parentName);
}
