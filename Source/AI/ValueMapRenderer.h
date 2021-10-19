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

//�f�o�b�O�p��ValueMap��\������N���X
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

		//�C���X�^���V���O�p����ǂݎ��
		for (auto& value : valueMap)
		{
			//�C���X�^���V���O�p����ǉ�
			valueMapInstances.emplace_back();
			auto& instance = valueMapInstances.back();

			const float scale = 0.02f;
			DirectX::XMMATRIX translation = DirectX::XMMatrixTranslationFromVector(value.m_Center.toXMVector());
			DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(value.m_Size * scale, 0.1f * scale, value.m_Size * scale);
			auto instanceMat = DirectX::XMMatrixTranspose(scaling * translation);

			//�s���K�p
			DirectX::XMStoreFloat4x4(&instance.instanceMat, instanceMat);

			float hoge = value.m_Value;
			Color color(m_Color.r * hoge / 100.0f,
				m_Color.g * hoge / 100.0f,
				m_Color.b * hoge / 100.0f,
				m_Color.a);

			//�F��K�p
			DirectX::XMStoreFloat4(&instance.instanceColor, color.toXMVECTOR());
		}

		//�C���X�^���X����K�p
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

	//�`��pInstancedRenderer�I�u�W�F�N�g����
	auto pObj = ModelGameObjectHelper::instantiateModel<ValueMapInstance>(
		getUser().getGameMediator(),
		GameDevice::getModelManager().getModel("Cube"),
		true);

	//InstancedRenderer�̊e��ݒ�
	pObj->setParent(&getUser());
	m_pInstancedRenderer = pObj->getChildren().at(0)->getComponent<InstancedRenderer<ValueMapInstance>>();
	m_pInstancedRenderer->setMaterial(pMaterial);
	pMaterial->setMainTexture(GameDevice::getTextureManager().getTexture("CircleFill"));

	//�F�ݒ�
	m_Color = color;
	m_Color.a = 1.0f;
}