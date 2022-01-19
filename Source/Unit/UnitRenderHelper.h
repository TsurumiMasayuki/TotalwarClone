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

	//インスタンス用情報を連結する(Transpose不要)
	void appendInstanceInfo(const std::vector<DirectX::XMMATRIX>& objMatrices);
	//インスタンス用情報をRendererに転送
	void sendInstanceInfo();

private:
	//ユニットのステータス
	const UnitStats* m_pUnitStats;
	//オブジェクトごとのデータ
	std::vector<DirectX::XMMATRIX> m_DataPerObject;
	//描画するInstancedRenderer
	InstancedRenderer<UnitInstanceInfo>* m_pInstancedRenderer;

	//Blockbenchモデル
	const BlockbenchModel* m_pBBModel;
	//Blockbenchモデル用マテリアル
	BBModelMaterial* m_pMaterial;


	//前フレームのオブジェクト数
	int m_PreviousObjectCount;
};