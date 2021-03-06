#pragma once
#include <vector>
#include <DirectXMath.h>

#include "AI\ValueMap.h"
#include "Component\Base\AbstractComponent.h"
#include "Component\Graphics\InstancedRenderer.h"
#include "Graphics\Material\ValueMapMaterial.h"
#include "Utility\Color.h"
#include "Utility\ModelGameObjectHelper.h"

class GameObject;

struct ValueMapInstance
{
	DirectX::XMFLOAT4X4 instanceMat;
	DirectX::XMFLOAT4 instanceColor;
};

//デバッグ用にValueMapを表示するクラス
template <UnitStatsValues T> class ValueMapRenderer
	: public AbstractComponent
{
public:
	virtual void onStart() override
	{
	}

	virtual void onUpdate() override
	{
		if (m_pValueMap == nullptr) return;

		const auto& valueMaps = m_pValueMap->getValueMaps();
		const auto& valueMap = valueMaps.at(T);

		std::vector<ValueMapInstance> valueMapInstances;
		valueMapInstances.reserve(valueMap.size());

		//インスタンシング用情報を読み取り
		for (auto& value : valueMap)
		{
			//インスタンシング用情報を追加
			valueMapInstances.emplace_back();
			auto& instance = valueMapInstances.back();

			const float scale = 0.02f;
			DirectX::XMMATRIX translation = DirectX::XMMatrixTranslationFromVector(value.m_Center.toXMVector());
			DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(value.m_Size * scale, 0.1f * scale, value.m_Size * scale);
			auto instanceMat = DirectX::XMMatrixTranspose(scaling * translation);

			//行列を適用
			DirectX::XMStoreFloat4x4(&instance.instanceMat, instanceMat);

			float hoge = value.m_Value;
			Color color(m_Color.r * hoge / 10000.0f,
				m_Color.g * hoge / 10000.0f,
				m_Color.b * hoge / 10000.0f,
				m_Color.a);

			//色を適用
			DirectX::XMStoreFloat4(&instance.instanceColor, color.toXMVECTOR());
		}

		//インスタンス情報を適用
		m_pInstancedRenderer->setInstanceInfo(valueMapInstances);
	}

	void init(ValueMap* pValueMap, ValueMapMaterial* pMaterial, const Color& color);

private:
	InstancedRenderer<ValueMapInstance>* m_pInstancedRenderer;
	ValueMap* m_pValueMap;
	Color m_Color;
};

template<UnitStatsValues T>
inline void ValueMapRenderer<T>::init(ValueMap* pValueMap, ValueMapMaterial* pMaterial, const Color& color)
{
	m_pValueMap = pValueMap;

	//描画用InstancedRendererオブジェクト生成
	auto pObj = ModelGameObjectHelper::instantiateModel<ValueMapInstance>(
		getUser().getGameMediator(),
		GameDevice::getModelManager().getModel("Cube"),
		true);

	//InstancedRendererの各種設定
	pObj->setParent(&getUser());
	m_pInstancedRenderer = pObj->getChildren().at(0)->getComponent<InstancedRenderer<ValueMapInstance>>();
	m_pInstancedRenderer->setMaterial(pMaterial);
	pMaterial->setMainTexture(GameDevice::getTextureManager().getTexture("CircleFill"));

	//色設定
	m_Color = color;
	m_Color.a = 1.0f;
}