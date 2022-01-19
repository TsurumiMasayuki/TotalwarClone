#pragma once
#include <vector>
#include "Component\Graphics\InstancedRenderer.h"
#include "Math\Vec3.h"

#include "Unit\UnitInstanceInfo.h"
#include "Utility\Color.h"

struct UnitStats;
class BlockbenchModel;
class BBModelMaterial;

class UnitRenderHelper
{
public:
	UnitRenderHelper(const UnitStats* pUnitStats, const BlockbenchModel* pModel, InstancedRenderer<UnitInstanceInfo>* pInstancedRenderer);
	~UnitRenderHelper();

	//�C���X�^���X�p����A������(Transpose�s�v)
	void appendInstanceInfo(const std::vector<DirectX::XMMATRIX>& objMatrices);
	//�C���X�^���X�p����Renderer�ɓ]��
	void sendInstanceInfo();

private:
	//���j�b�g�̃X�e�[�^�X
	const UnitStats* m_pUnitStats;
	//�I�u�W�F�N�g���Ƃ̃f�[�^
	std::vector<DirectX::XMMATRIX> m_DataPerObject;
	//�`�悷��InstancedRenderer
	InstancedRenderer<UnitInstanceInfo>* m_pInstancedRenderer;

	//Blockbench���f��
	const BlockbenchModel* m_pBBModel;
	//Blockbench���f���p�}�e���A��
	BBModelMaterial* m_pMaterial;


	//�O�t���[���̃I�u�W�F�N�g��
	int m_PreviousObjectCount;
};