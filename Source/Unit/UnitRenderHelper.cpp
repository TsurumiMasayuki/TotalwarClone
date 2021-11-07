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

	//インスタンシング用情報を連結
	for (const auto& matrix : objMatrices)
	{
		m_InstanceInfo.emplace_back();
		auto& instance = m_InstanceInfo.back();

		//インスタンシング用情報を設定(UV系は後ほど)
		DirectX::XMStoreFloat4x4(&instance.instanceMat, DirectX::XMMatrixTranspose(matrix));
		DirectX::XMStoreFloat4(&instance.instanceColor, color);
	}
}

void UnitRenderHelper::sendInstanceInfo()
{
	//インスタンシング用情報を転送
	m_pInstancedRenderer->setInstanceInfo(m_InstanceInfo);
	//転送したのでクリア
	m_InstanceInfo.clear();
}
