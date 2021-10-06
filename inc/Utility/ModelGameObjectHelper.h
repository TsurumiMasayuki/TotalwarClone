#pragma once
#include <Actor\Base\GameObject.h>
#include <Device\Resource\Model.h>
#include <Component\Graphics\MeshRenderer.h>
#include <Component\Graphics\InstancedRenderer.h>
#include <Math\MathUtility.h>

class ModelGameObjectHelper
{
public:
	template<typename T>
	static GameObject* instantiateModel(IGameMediator* pGameMediator, Model* pModel, bool isInstancing = false);
private:
	template<typename T>
	static void instantiateNodeRecursive(GameObject* pParent, IGameMediator* pGameMediator, Node& node, Model * pModel, bool isInstancing);
};

template<typename T>
GameObject* ModelGameObjectHelper::instantiateModel(IGameMediator* pGameMediator, Model* pModel, bool isInstancing)
{
	int size = (int)pModel->m_Root.m_Children.size();

	auto pGameObject = new GameObject(pGameMediator);

	for (int i = 0; i < size; ++i)
		instantiateNodeRecursive<T>(pGameObject, pGameMediator, pModel->m_Root.m_Children.at(i), pModel, isInstancing);

	return pGameObject;
}

template<typename T>
void ModelGameObjectHelper::instantiateNodeRecursive(GameObject* pParent, IGameMediator* pGameMediator, Node& node, Model* pModel, bool isInstancing)
{
	//子オブジェクトが無い+メッシュが無いなら何も生成しない
	if (node.m_Children.size() == 0 && !node.m_pMesh)
		return;

	auto pGameObject = new GameObject(pGameMediator);
	pGameObject->setParent(pParent);

	auto& transform = pGameObject->getTransform();
	transform.setLocalPosition(Vec3(node.m_Translation));
	Vec3 angles;
	angles.x = node.m_Rotation.m128_f32[0];
	angles.y = node.m_Rotation.m128_f32[1];
	angles.z = node.m_Rotation.m128_f32[2];
	transform.setLocalAngles(angles);
	transform.setLocalScale(Vec3(node.m_Scaling));

	if (node.m_pMesh)
	{
		if (!isInstancing)
		{
			auto pRenderer = pGameObject->addComponent<MeshRenderer>();
			pRenderer->setMesh(node.m_pMesh);
			pRenderer->setTextureByName(node.m_pMesh->getTextureName());
			pRenderer->setMetalness(pModel->m_Metalness);
			pRenderer->setSpecular(pModel->m_Specular);
			pRenderer->setRoughness(pModel->m_Roughness);
			pRenderer->setColor(Color(pModel->m_BaseColor.x, pModel->m_BaseColor.y, pModel->m_BaseColor.z, 1.0f));
		}
		else
		{
			auto pRenderer = pGameObject->addComponent<InstancedRenderer<T>>();
			pRenderer->setMesh(node.m_pMesh);
			pRenderer->setTextureByName(node.m_pMesh->getTextureName());
		}
	}

	int size = (int)node.m_Children.size();
	for (int i = 0; i < size; ++i)
		instantiateNodeRecursive<T>(pGameObject, pGameMediator, node.m_Children.at(i), pModel, isInstancing);
}