#include "UnitRenderHelper.h"

#include "Blockbench\BlockbenchModel.h"
#include "Unit\UnitStats.h"

UnitRenderHelper::UnitRenderHelper(const UnitStats* pUnitStats, BlockbenchModel* pModel, InstancedRenderer<UnitInstanceInfo>* pInstancedRenderer)
	: m_pUnitStats(pUnitStats),
	m_pModel(pModel),
	m_pInstancedRenderer(pInstancedRenderer)
{
}

void UnitRenderHelper::appendInstanceInfo(const std::vector<DirectX::XMMATRIX>& objMatrices)
{
	DirectX::XMVECTOR color = m_pUnitStats->m_DebugColor.toXMVECTOR();

	//�C���X�^���V���O�p����A��
	for (const auto& matrix : objMatrices)
	{
		m_InstanceInfo.emplace_back();
		auto& instance = m_InstanceInfo.back();

		//�C���X�^���V���O�p����ݒ�(UV�n�͌�ق�)
		DirectX::XMStoreFloat4x4(&instance.instanceMat, DirectX::XMMatrixTranspose(matrix));
		DirectX::XMStoreFloat4(&instance.instanceColor, color);
	}
}

void UnitRenderHelper::sendInstanceInfo()
{
	//�C���X�^���V���O�p����]��
	m_pInstancedRenderer->setInstanceInfo(m_InstanceInfo);
	//�]�������̂ŃN���A
	m_InstanceInfo.clear();
}
