#include "EffectRenderHelper.h"

EffectRenderHelper::EffectRenderHelper(InstancedRenderer<EffectInstanceInfo>* pInstancedRenderer)
	: m_pInstancedRenderer(pInstancedRenderer)
{
}

void EffectRenderHelper::appendInstanceInfoSingle(const DirectX::XMMATRIX& objMatrix)
{
	m_InstanceInfo.emplace_back();
	auto& instance = m_InstanceInfo.back();

	//インスタンシング用情報を設定(UV系は後ほど)
	DirectX::XMStoreFloat4x4(&instance.instanceMat, DirectX::XMMatrixTranspose(objMatrix));
	DirectX::XMStoreFloat4(&instance.instanceColor, DirectX::Colors::Yellow);
}

void EffectRenderHelper::appendInstanceInfo(const std::vector<DirectX::XMMATRIX>& objMatrices)
{
	//インスタンシング用情報を連結
	for (const auto& matrix : objMatrices)
	{
		m_InstanceInfo.emplace_back();
		auto& instance = m_InstanceInfo.back();

		DirectX::XMStoreFloat4x4(&instance.instanceMat, DirectX::XMMatrixTranspose(matrix));
		DirectX::XMStoreFloat4(&instance.instanceColor, DirectX::Colors::Yellow);
	}
}

void EffectRenderHelper::sendInstanceInfo()
{
	//インスタンシング用情報を転送
	m_pInstancedRenderer->setInstanceInfo(m_InstanceInfo);
	//転送したのでクリア
	m_InstanceInfo.clear();
}