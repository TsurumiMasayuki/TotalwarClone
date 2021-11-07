#pragma once
#include <vector>
#include "Component\Graphics\InstancedRenderer.h"
#include "Math\Vec3.h"

#include "Unit\UnitInstanceInfo.h"
#include "Utility\Color.h"

struct UnitStats;
class BlockbenchModel;

class UnitRenderHelper
{
public:
	UnitRenderHelper(const UnitStats* pUnitStats, BlockbenchModel* pModel, InstancedRenderer<UnitInstanceInfo>* pInstancedRenderer);

	//�C���X�^���X�p����A������(Transpose�s�v)
	void appendInstanceInfo(const std::vector<DirectX::XMMATRIX>& objMatrices);
	//�C���X�^���X�p����Renderer�ɓ]��
	void sendInstanceInfo();

private:
	//���j�b�g�̃X�e�[�^�X
	const UnitStats* m_pUnitStats;
	//���f��
	BlockbenchModel* m_pModel;
	//InstancedRenderer�ɑ���p�̏��
	std::vector<UnitInstanceInfo> m_InstanceInfo;
	//�`�悷��InstancedRenderer
	InstancedRenderer<UnitInstanceInfo>* m_pInstancedRenderer;
};