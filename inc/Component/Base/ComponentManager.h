#pragma once
#include <vector>
#include <cassert>
#include <Component\Base\AbstractComponent.h>
#include <Component\Utility\Transform.h>
#include <Utility\ManagerHelper.h>


class GameObject;

class ComponentManager
{
public:
	ComponentManager(GameObject* pUser);
	~ComponentManager();

	void update();

	void onEnable();
	void onDisable();
	void onDestroy();

	void onCollisionEnter(GameObject* pHit);
	void onCollisionStay(GameObject* pHit);
	void onCollisionExit(GameObject* pHit);

	void onTriggerEnter(GameObject* pHit);
	void onTriggerStay(GameObject* pHit);
	void onTriggerExit(GameObject* pHit);

	//�R���|�[�l���g���擾
	template<typename T>
	T * getComponent();

	template<typename T>
	T * addComponent();
	template<typename T>
	void removeComponent();

	const std::vector<AbstractComponent*>& getAllComponents();

private:
	GameObject* m_pUser;
	ManagerHelper<AbstractComponent*> m_Components;
};

template<typename T>
inline T * ComponentManager::getComponent()
{
	T* pComponent = nullptr;

	for (auto& component : m_Components.getVector())
	{
		pComponent = dynamic_cast<T*>(component);
		//�L���X�g�ɐ��������烋�[�v�I��
		if (pComponent != nullptr) break;
	}

	//�������Ă�����I��
	if (pComponent != nullptr) return pComponent;

	for (auto& component : m_Components.getAddQueueVector())
	{
		pComponent = dynamic_cast<T*>(component);
		//�L���X�g�ɐ��������烋�[�v�I��
		if (pComponent != nullptr) break;
	}

	return pComponent;
}

template<typename T>
inline T * ComponentManager::addComponent()
{
	T* pNewComponent = new T();

#ifdef _DEBUG
	//AbstractComponent�̔h���N���X���`�F�b�N����
	auto pCheck = dynamic_cast<AbstractComponent*>(pNewComponent);
	assert(pCheck != nullptr);
#endif

	m_Components.addQueue(pNewComponent);
	pNewComponent->setUser(*m_pUser);

	//�ŏ��̃R���|�[�l���g(Transform)�łȂ��Ȃ�
	if (m_Components.getAddQueueVector().size() != 1)
		pNewComponent->getTransform().addListner(pNewComponent);

	return pNewComponent;
}

template<typename T>
inline void ComponentManager::removeComponent()
{
	auto remove = getComponent<T>();
#ifdef _DEBUG
	//�擾�ł������`�F�b�N
	assert(remove != nullptr);
#endif
	remove->getTransform().removeListner(remove);
	m_Components.remove(remove);
}
