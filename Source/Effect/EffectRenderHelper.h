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

	//インスタンス用情報を連結する(単体。Transpose不要)
	void appendInstanceInfoSingle(const DirectX::XMMATRIX& objMatrix, const Color& color = Color(DirectX::Colors::Yellow));
	//インスタンス用情報を連結する(Transpose不要)
	void appendInstanceInfo(const std::vector<DirectX::XMMATRIX>& objMatrices, const Color& color = Color(DirectX::Colors::Yellow));
	//インスタンス用情報をRendererに転送
	void sendInstanceInfo();

private:
	//InstancedRendererに送る用の情報
	std::vector<EffectInstanceInfo> m_InstanceInfo;
	//描画するInstancedRenderer
	InstancedRenderer<EffectInstanceInfo>* m_pInstancedRenderer;
};