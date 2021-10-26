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
	std::vector<XMMATRIX> resultMatrices;
	std::vector<UV> resultUVOrigins;
	std::vector<UV> resultUVSizes;

	//���f���̏����擾
	auto& description = jsonFile["minecraft:geometry"][0]["description"];
	//�e�N�X�`���̕�
	float textureWidth = description["texture_width"];
	//�e�N�X�`���̍���
	float textureHeight = description["texture_height"];

	//�{�[�����X�g���擾
	auto& bones = jsonFile["minecraft:geometry"][0]["bones"];

	//�I�t�Z�b�g
	DirectX::XMMATRIX modelOffset = DirectX::XMMatrixTranslation(0.0f, -50.0f, 0.0f);

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
			bonePivotMat = XMMatrixTranslation(-(float)(bonePivot[0] - 1.0f) * 0.5f, -(float)(bonePivot[1]), (float)(bonePivot[2]));
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
				scaling = XMMatrixScalingFromVector((Vec3(size[0], size[1], size[2]) * 0.05f).toXMVector());
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
				pivotMat = XMMatrixTranslation(pivot[0] - 0.5f, pivot[1], pivot[2]);
			}

			//�X�P�[����1/2�����炷
			XMMATRIX offsetMat = XMMatrixTranslation(size[0] / 2, size[1] / 2, size[2] / 2);

			//�s���S�č���
			XMMATRIX world = modelOffset * scaling * parentMatrix * rotationMat * pivotMat * translation * offsetMat;
			resultMatrices.emplace_back(world);

			//UV��ǂݍ���
			float originX = (float)cube["uv"][0] / textureWidth;
			float originY = (float)cube["uv"][1] / textureHeight;

			float xSize = (float)size[0];
			float ySize = (float)size[1];
			float zSize = (float)size[2];

			Face faces[6] = 
			{
				//+Z�̖�
				Face(UV(originX + zSize / textureWidth, originY + zSize / textureHeight),
					xSize / textureWidth,
					ySize / textureHeight),
				//+X�̖�
				Face(UV(originX, originY + zSize / textureHeight),
					zSize / textureWidth,
					ySize / textureHeight),
				//-X�̖�
				Face(UV(originX + (xSize + zSize) / textureWidth, originY + zSize / textureHeight),
					zSize / textureWidth,
					ySize / textureHeight),
				//-Z�̖�
				Face(UV(originX + (xSize + zSize * 2) / textureWidth, originY + zSize / textureHeight),
					xSize / textureWidth,
					ySize / textureHeight),
				//+Y�̖�
				Face(UV(originX + zSize / textureWidth, originY),
					xSize / textureWidth,
					zSize / textureHeight),
				//-Y�̖�
				Face(UV(originX + (xSize + zSize) / textureWidth, originY),
					xSize / textureWidth,
					zSize / textureHeight)
			};

			//�ʂ��ƂɃ��[�v
			for (int i = 0; i < BlockbenchModel::cubeFaceCount; i++)
			{
				resultUVOrigins.emplace_back();
				resultUVSizes.emplace_back();

				resultUVOrigins.back() = faces[i].uvUL;
				resultUVSizes.back() = UV(faces[i].uvDR.x - faces[i].uvUL.x, faces[i].uvDR.y - faces[i].uvUL.y);
			}
		}
	}

	//���f����o�^
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
