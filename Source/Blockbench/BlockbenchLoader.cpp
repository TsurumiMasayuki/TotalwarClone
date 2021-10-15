#include "BlockbenchLoader.h"
#include <fstream>
#include "Math\Vec3.h"
#include "Math\MathUtility.h"
#include "Blockbench\BlockbenchModel.h"

using namespace DirectX;
using namespace nlohmann;

void BlockbenchLoader::loadModel(const std::string& filePath, const std::string& key)
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

	//ボーンリストを取得
	auto& bones = jsonFile["minecraft:geometry"][0]["bones"];

	//ボーンごとに処理
	for (auto& bone : bones)
	{
		//ボーン内の立方体リストを取得
		json cubes;
		if (!safeGet(bone, cubes, "cubes"))
			break;

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
			bonePivotMat = XMMatrixTranslation(bonePivot[0], bonePivot[1], bonePivot[2]);
		}

		//行列を合成
		XMMATRIX boneMat = boneRotationMat * bonePivotMat;

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
				scaling = XMMatrixScalingFromVector((Vec3(size[0], size[1], size[2]) * 0.02f).toXMVector());
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
				pivotMat = XMMatrixTranslation(pivot[0], pivot[1], pivot[2]);
			}

			//行列を全て合成
			XMMATRIX world = scaling * parentMatrix * pivotMat * rotationMat * translation;
			resultMatrices.emplace_back(world);
		}
	}

	//モデルを登録
	m_Models.emplace(key, new BlockbenchModel(resultMatrices));
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
