#pragma once
#include <vector>
#include <DirectXMath.h>

#include "AI\ValueMap.h"
#include "Component\Base\AbstractComponent.h"
#include "Component\Graphics\InstancedRenderer.h"
#include "Graphics\DX12\Material\DefaultMaterials.h"
#include "Utility\Color.h"
#include "Utility\ModelGameObjectHelper.h"

class GameObject;

struct ValueMapInstance
{
	DirectX::XMFLOAT4X4 instanceMat;
	DirectX::XMFLOAT4 instanceColor;
};

template <ValueTypes T> class ValueMapRenderer
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
			//色を適用
			DirectX::XMStoreFloat4(&instance.instanceColor, m_Color.toXMVECTOR());
		}

		//インスタンス情報を適用
		m_pInstancedRenderer->setInstanceInfo(valueMapInstances);
	}

	void init(ValueMap* pValueMap, InstancingMaterial* pMaterial, const Color& color);

private:
	InstancedRenderer<ValueMapInstance>* m_pInstancedRenderer;
	ValueMap* m_pValueMap;
	Color m_Color;
};

template<ValueTypes T>
inline void ValueMapRenderer<T>::init(ValueMap* pValueMap, InstancingMaterial* pMaterial, const Color& color)
{
	m_pValueMap = pValueMap;

	auto pObj = ModelGameObjectHelper::instantiateModel<ValueMapInstance>(
		getUser().getGameMediator(),
		GameDevice::getModelManager().getModel("Cube"),
		true);

	pObj->setParent(&getUser());
	m_pInstancedRenderer = pObj->getChildren().at(0)->getComponent<InstancedRenderer<ValueMapInstance>>();
	m_pInstancedRenderer->setMaterial(pMaterial);
	pMaterial->setMainTexture(GameDevice::getTextureManager().getTexture("CircleFill"));

	m_Color = color;
	m_Color.a = 0.6f;
}