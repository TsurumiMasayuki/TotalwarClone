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
	std::vector<Cube> resultCubes;

	//モデルの情報を取得
	auto& description = jsonFile["minecraft:geometry"][0]["description"];
	//テクスチャの幅
	float textureWidth = description["texture_width"];
	//テクスチャの高さ
	float textureHeight = description["texture_height"];

	//ボーンリストを取得
	auto& bones = jsonFile["minecraft:geometry"][0]["bones"];

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
			bonePivotMat = XMMatrixTranslation((float)bonePivot[0], (float)bonePivot[1], (float)bonePivot[2]);
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
			XMMATRIX translateMat = XMMatrixIdentity();
			if (safeGet(cube, origin, "origin"))
			{
				translateMat = XMMatrixTranslation(origin[0], origin[1], origin[2]);
			}

			//スケール情報を読み取って行列に変換
			json size;
			XMMATRIX scalingMat = XMMatrixIdentity();
			if (safeGet(cube, size, "size"))
			{
				scalingMat = XMMatrixScalingFromVector((Vec3(size[0], size[1], size[2])).toXMVector());
			}

			//回転情報を読み取って行列に変換
			json rotation;
			XMMATRIX rotationMat = XMMatrixIdentity();
			if (safeGet(cube, rotation, "rotation"))
			{
				rotationMat = XMMatrixRotationRollPitchYaw(
					-MathUtility::toRadian(rotation[0]),
					MathUtility::toRadian(rotation[1]),
					-MathUtility::toRadian(rotation[2])
				);
			}

			//ピボット情報を読み取って行列に変換
			json pivot;
			XMMATRIX pivotMat = XMMatrixIdentity();
			XMMATRIX pivotReverseMat = XMMatrixIdentity();
			if (safeGet(cube, pivot, "pivot"))
			{
				pivotMat = XMMatrixTranslation((float)pivot[0], (float)pivot[1], (float)pivot[2]);
				pivotReverseMat = XMMatrixTranslation(-(float)pivot[0], -(float)pivot[1], -(float)pivot[2]);
			}

			//行列を全て合成
			XMMATRIX world = scalingMat * translateMat * pivotReverseMat * rotationMat * pivotMat;

			//UVを読み込み
			float originX = (float)cube["uv"][0];
			float originY = (float)cube["uv"][1];

			//切り捨てしながらピクセル数算出
			float xSize = (float)(int)size[0];
			float ySize = (float)(int)size[1];
			float zSize = (float)(int)size[2];

			Face faces[6] = 
			{
				//+Yの面
				Face(UV((originX + zSize) / textureWidth, originY / textureHeight),
					xSize / textureWidth,
					zSize / textureHeight),
				//-Zの面
				Face(UV((originX + zSize) / textureWidth, (originY + zSize) / textureHeight),
					xSize / textureWidth,
					ySize / textureHeight),
				//+Xの面
				Face(UV((originX + xSize + zSize) / textureWidth, (originY + zSize) / textureHeight),
					zSize / textureWidth,
					ySize / textureHeight),
				//-Yの面
				Face(UV((originX + xSize + zSize) / textureWidth, originY / textureHeight),
					xSize / textureWidth,
					zSize / textureHeight),
				//-Xの面
				Face(UV(originX / textureWidth, (originY + zSize) / textureHeight),
					zSize / textureWidth,
					ySize / textureHeight),
				//+Zの面
				Face(UV((originX + xSize + zSize * 2) / textureWidth, (originY + zSize) / textureHeight),
					xSize / textureWidth,
					ySize / textureHeight)
			};

			UV resultUVs[36];

			for (int i = 0; i < 36; i += 6)
			{
				//1ポリゴン分設定
				resultUVs[i + 0] = faces[i / 6].uvDL;
				resultUVs[i + 1] = faces[i / 6].uvUL;
				resultUVs[i + 2] = faces[i / 6].uvUR;

				//1ポリゴン分設定
				resultUVs[i + 3] = faces[i / 6].uvUR;
				resultUVs[i + 4] = faces[i / 6].uvDR;
				resultUVs[i + 5] = faces[i / 6].uvDL;
			}

			resultCubes.emplace_back(Cube(resultUVs, world));
		}
	}

	//モデルを登録
	m_Models.emplace(key, new BlockbenchModel(resultCubes, textureName, (unsigned int)resultCubes.size()));
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
