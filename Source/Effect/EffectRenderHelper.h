#pragma once
#include <vector>
#include "Component\Graphics\InstancedRenderer.h"
#include "Math\Vec3.h"

#include "Unit\UnitInstanceInfo.h"
#include "Utility\Color.h"

struct EffectInstanceInfo
{
	DirectX::XMFLOAT4X4 instanceMat;
	DirectX::XMFLOAT4 instanceColor;
};

class EffectRenderHelper
{
public:
	EffectRenderHelper(InstancedRenderer<EffectInstanceInfo>* pInstancedRenderer);

	//�C���X�^���X�p����A������(�P�́BTranspose�s�v)
	void appendInstanceInfoSingle(const DirectX::XMMATRIX& objMatrix, const Color& color = Color(DirectX::Colors::Yellow));
	//�C���X�^���X�p����A������(Transpose�s�v)
	void appendInstanceInfo(const std::vector<DirectX::XMMATRIX>& objMatrices, const Color& color = Color(DirectX::Colors::Yellow));
	//�C���X�^���X�p����Renderer�ɓ]��
	void sendInstanceInfo();

private:
	//InstancedRenderer�ɑ���p�̏��
	std::vector<EffectInstanceInfo> m_InstanceInfo;
	//�`�悷��InstancedRenderer
	InstancedRenderer<EffectInstanceInfo>* m_pInstancedRenderer;
};