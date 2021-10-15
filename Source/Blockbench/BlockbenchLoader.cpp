#include "BlockbenchLoader.h"
#include <fstream>
#include "Math\Vec3.h"
#include "Math\MathUtility.h"
#include "Blockbench\BlockbenchModel.h"

using namespace DirectX;
using namespace nlohmann;

void BlockbenchLoader::loadModel(const std::string& filePath, const std::string& key)
{
	//�t�@�C����ǂݍ���
	std::fstream fs(filePath);
	assert(fs.is_open());

	//�ǂݍ��񂾃t�@�C����json�ɕϊ�
	json jsonFile;
	fs >> jsonFile;

	//�{�[�����ۑ��pmap
	std::unordered_map<std::string, BoneInfo> boneInfo;

	//���ʗpvector
	std::vector<XMMATRIX> resultMatrices;

	//�{�[�����X�g���擾
	auto& bones = jsonFile["minecraft:geometry"][0]["bones"];

	//�{�[�����Ƃɏ���
	for (auto& bone : bones)
	{
		//�{�[�����̗����̃��X�g���擾
		json cubes;
		if (!safeGet(bone, cubes, "cubes"))
			break;

		//�{�[���̏����擾
		json boneName;
		boneName = bone["name"];

		//��]����ǂݎ���čs��ɕϊ�
		json boneRotation;
		XMMATRIX boneRotationMat = XMMatrixIdentity();
		if (safeGet(bone, boneRotation, "rotation"))
		{
			boneRotationMat = XMMatrixRotationRollPitchYaw(MathUtility::toRadian(boneRotation[0]),
				MathUtility::toRadian(boneRotation[1]),
				MathUtility::toRadian(boneRotation[2])
			);
		}

		//�s�{�b�g����ǂݎ���čs��ɕϊ�
		json bonePivot;
		XMMATRIX bonePivotMat = XMMatrixIdentity();
		if (safeGet(bone, bonePivot, "pivot"))
		{
			bonePivotMat = XMMatrixTranslation(bonePivot[0], bonePivot[1], bonePivot[2]);
		}

		//�s�������
		XMMATRIX boneMat = boneRotationMat * bonePivotMat;

		//�e�̖��O��ǂݎ���Đe�̍s�����������
		json parentName = "";
		XMMATRIX parentMatrix = boneMat;
		if (safeGet(bone, parentName, "parentName"))
		{
			multiplyParentMatrix(parentMatrix, boneInfo, parentName);
		}

		//�{�[���s���ۑ�
		boneInfo.emplace(boneName, BoneInfo(parentName, boneMat));

		//�����̂��Ƃɏ���
		for (auto& cube : cubes)
		{
			//���W����ǂݎ���čs��ɕϊ�
			json origin;
			XMMATRIX translation = XMMatrixIdentity();
			if (safeGet(cube, origin, "origin"))
			{
				translation = XMMatrixTranslation(origin[0], origin[1], origin[2]);
			}

			//�X�P�[������ǂݎ���čs��ɕϊ�
			json size;
			XMMATRIX scaling = XMMatrixIdentity();
			if (safeGet(cube, size, "size"))
			{
				scaling = XMMatrixScalingFromVector((Vec3(size[0], size[1], size[2]) * 0.02f).toXMVector());
			}

			//��]����ǂݎ���čs��ɕϊ�
			json rotation;
			XMMATRIX rotationMat = XMMatrixIdentity();
			if (safeGet(cube, rotation, "rotation"))
			{
				rotationMat = XMMatrixRotationRollPitchYaw(MathUtility::toRadian(rotation[0]),
					MathUtility::toRadian(rotation[1]),
					MathUtility::toRadian(rotation[2])
				);
			}

			//�s�{�b�g����ǂݎ���čs��ɕϊ�
			json pivot;
			XMMATRIX pivotMat = XMMatrixIdentity();
			if (safeGet(cube, pivot, "pivot"))
			{
				pivotMat = XMMatrixTranslation(pivot[0], pivot[1], pivot[2]);
			}

			//�s���S�č���
			XMMATRIX world = scaling * parentMatrix * pivotMat * rotationMat * translation;
			resultMatrices.emplace_back(world);
		}
	}

	//���f����o�^
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
	//key���܂܂�Ă��邩�ǂ���
	bool contains = source.contains(key);

	//�܂܂�Ă�����Get
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
	//�w�肳�ꂽ�e�{�[���̖��O�������Ȃ�return
	if (!boneInfo.contains(parentName)) return;

	auto& parent = boneInfo.at(parentName);

	//�e�̍s����|����
	result *= parent.boneMat;
	//�ċA�Ăяo��
	multiplyParentMatrix(result, boneInfo, parent.parentName);
}
