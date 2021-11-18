#include "EffectRenderHelper.h"

EffectRenderHelper::EffectRenderHelper(InstancedRenderer<EffectInstanceInfo>* pInstancedRenderer)
	: m_pInstancedRenderer(pInstancedRenderer)
{
}

void EffectRenderHelper::appendInstanceInfoSingle(const DirectX::XMMATRIX& objMatrix)
{
	m_InstanceInfo.emplace_back();
	auto& instance = m_InstanceInfo.back();

	//�C���X�^���V���O�p����ݒ�(UV�n�͌�ق�)
	DirectX::XMStoreFloat4x4(&instance.instanceMat, DirectX::XMMatrixTranspose(objMatrix));
	DirectX::XMStoreFloat4(&instance.instanceColor, DirectX::Colors::Yellow);
}

void EffectRenderHelper::appendInstanceInfo(const std::vector<DirectX::XMMATRIX>& objMatrices)
{
	//�C���X�^���V���O�p����A��
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
	//�C���X�^���V���O�p����]��
	m_pInstancedRenderer->setInstanceInfo(m_InstanceInfo);
	//�]�������̂ŃN���A
	m_InstanceInfo.clear();
}