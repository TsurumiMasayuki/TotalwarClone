#include "UnitRenderHelper.h"

#include "Blockbench\BlockbenchModel.h"
#include "Unit\UnitStats.h"
#include "Graphics\Material\BBModelMaterial.h"

UnitRenderHelper::UnitRenderHelper(const UnitStats* pUnitStats, const BlockbenchModel* pModel, InstancedRenderer<UnitInstanceInfo>* pInstancedRenderer)
	: m_pUnitStats(pUnitStats),
	m_pInstancedRenderer(pInstancedRenderer),
	m_PreviousObjectCount(-1),
	m_pBBModel(pModel),
	m_pMaterial(new BBModelMaterial())
{
	m_pMaterial->init(DX12GraphicsCore::g_pDevice.Get());
	m_pMaterial->setBBModel(*pModel);
	m_pInstancedRenderer->setMaterial(m_pMaterial);
}

UnitRenderHelper::~UnitRenderHelper()
{
	if (m_pMaterial != nullptr)
		delete m_pMaterial;
}

void UnitRenderHelper::appendInstanceInfo(const std::vector<DirectX::XMMATRIX>& objMatrices)
{
	//インスタンシング用情報を連結
	for (const auto& matrix : objMatrices)
	{
		m_DataPerObject.emplace_back(matrix);
	}
}

void UnitRenderHelper::sendInstanceInfo()
{
	int currentObjectCount = (int)m_DataPerObject.size();

	if (currentObjectCount == 0) return;

	//前フレームのオブジェクト数と比較して違っていたら更新
	if (currentObjectCount != m_PreviousObjectCount)
	{
		m_pMaterial->setModelCount(currentObjectCount);

		m_pMaterial->initDataPerCube();
		m_pMaterial->initDataPerObject();
	}

	//マテリアルにデータ送信
	m_pMaterial->updateDataPerObject(m_DataPerObject);

	m_pInstancedRenderer->setInstanceCount(currentObjectCount * m_pBBModel->getCubeCount());

	m_DataPerObject.clear();

	m_PreviousObjectCount = currentObjectCount;
}
