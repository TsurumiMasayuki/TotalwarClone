#include "BlockbenchLoader.h"
#include <fstream>

#include "Device\GameDevice.h"
#include "Math\Vec3.h"
#include "Math\MathUtility.h"

using namespace DirectX;
using namespace nlohmann;

void BlockbenchLoader::load(const std::string& filePath, const std::string& textureName, const std::string& key)
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
	std::vector<Cube> resultCubes;

	//���f���̏����擾
	auto& description = jsonFile["minecraft:geometry"][0]["description"];
	//�e�N�X�`���̕�
	float textureWidth = description["texture_width"];
	//�e�N�X�`���̍���
	float textureHeight = description["texture_height"];

	//�{�[�����X�g���擾
	auto& bones = jsonFile["minecraft:geometry"][0]["bones"];

	//�{�[�����Ƃɏ���
	for (auto& bone : bones)
	{
		//�{�[�����̗����̃��X�g���擾
		json cubes;
		if (!safeGet(bone, cubes, "cubes"))
			continue;

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
			bonePivotMat = XMMatrixTranslation((float)bonePivot[0], (float)bonePivot[1], (float)bonePivot[2]);
		}

		//�s�������
		XMMATRIX boneMat = bonePivotMat * boneRotationMat;

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
			XMMATRIX translateMat = XMMatrixIdentity();
			if (safeGet(cube, origin, "origin"))
			{
				translateMat = XMMatrixTranslation(origin[0], origin[1], origin[2]);
			}

			//�X�P�[������ǂݎ���čs��ɕϊ�
			json size;
			XMMATRIX scalingMat = XMMatrixIdentity();
			if (safeGet(cube, size, "size"))
			{
				scalingMat = XMMatrixScalingFromVector((Vec3(size[0], size[1], size[2])).toXMVector());
			}

			//��]����ǂݎ���čs��ɕϊ�
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

			//�s�{�b�g����ǂݎ���čs��ɕϊ�
			json pivot;
			XMMATRIX pivotMat = XMMatrixIdentity();
			XMMATRIX pivotReverseMat = XMMatrixIdentity();
			if (safeGet(cube, pivot, "pivot"))
			{
				pivotMat = XMMatrixTranslation((float)pivot[0], (float)pivot[1], (float)pivot[2]);
				pivotReverseMat = XMMatrixTranslation(-(float)pivot[0], -(float)pivot[1], -(float)pivot[2]);
			}

			//�s���S�č���
			XMMATRIX world = scalingMat * translateMat * pivotReverseMat * rotationMat * pivotMat;

			//UV��ǂݍ���
			float originX = (float)cube["uv"][0];
			float originY = (float)cube["uv"][1];

			//�؂�̂Ă��Ȃ���s�N�Z�����Z�o
			float xSize = (float)(int)size[0];
			float ySize = (float)(int)size[1];
			float zSize = (float)(int)size[2];

			Face faces[6] = 
			{
				//+Y�̖�
				Face(UV((originX + zSize) / textureWidth, originY / textureHeight),
					xSize / textureWidth,
					zSize / textureHeight),
				//-Z�̖�
				Face(UV((originX + zSize) / textureWidth, (originY + zSize) / textureHeight),
					xSize / textureWidth,
					ySize / textureHeight),
				//+X�̖�
				Face(UV((originX + xSize + zSize) / textureWidth, (originY + zSize) / textureHeight),
					zSize / textureWidth,
					ySize / textureHeight),
				//-Y�̖�
				Face(UV((originX + xSize + zSize) / textureWidth, originY / textureHeight),
					xSize / textureWidth,
					zSize / textureHeight),
				//-X�̖�
				Face(UV(originX / textureWidth, (originY + zSize) / textureHeight),
					zSize / textureWidth,
					ySize / textureHeight),
				//+Z�̖�
				Face(UV((originX + xSize + zSize * 2) / textureWidth, (originY + zSize) / textureHeight),
					xSize / textureWidth,
					ySize / textureHeight)
			};

			UV resultUVs[36];

			for (int i = 0; i < 36; i += 6)
			{
				//1�|���S�����ݒ�
				resultUVs[i + 0] = faces[i / 6].uvDL;
				resultUVs[i + 1] = faces[i / 6].uvUL;
				resultUVs[i + 2] = faces[i / 6].uvUR;

				//1�|���S�����ݒ�
				resultUVs[i + 3] = faces[i / 6].uvUR;
				resultUVs[i + 4] = faces[i / 6].uvDR;
				resultUVs[i + 5] = faces[i / 6].uvDL;
			}

			resultCubes.emplace_back(Cube(resultUVs, world));
		}
	}

	//���f����o�^
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
