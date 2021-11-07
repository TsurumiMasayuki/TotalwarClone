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

	//インスタンス用情報を連結する(Transpose不要)
	void appendInstanceInfo(const std::vector<DirectX::XMMATRIX>& objMatrices);
	//インスタンス用情報をRendererに転送
	void sendInstanceInfo();

private:
	//ユニットのステータス
	const UnitStats* m_pUnitStats;
	//モデル
	BlockbenchModel* m_pModel;
	//InstancedRendererに送る用の情報
	std::vector<UnitInstanceInfo> m_InstanceInfo;
	//描画するInstancedRenderer
	InstancedRenderer<UnitInstanceInfo>* m_pInstancedRenderer;
};