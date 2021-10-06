#pragma once
#include <vector>
#include <string>
#include <Component\Base\ComponentManager.h>
#include <Component\Utility\ITransformEventListner.h>
#include <Scene\Base\IGameMediator.h>
#include <Utility\INonCopyable.h>

class AbstractComponent;
class IGameMediator;
class Transform;

namespace Action
{
	class ActionManager;
}

class GameObject :
	private INonCopyable, public ITransformEventListner
{
public:
	GameObject(IGameMediator* pGameMediator);
	virtual ~GameObject();

	void baseUpdate();

	//�e�I�u�W�F�N�g��ݒ�(nullptr�ŉ���)
	void setParent(GameObject* parent);
	//�e�I�u�W�F�N�g���擾
	GameObject* getParent();

	//�q�I�u�W�F�N�g��o�^
	void addChild(GameObject& child);
	//�q�I�u�W�F�N�g��o�^����
	void removeChild(GameObject& child);
	//�q�I�u�W�F�N�g��S�擾
	const std::vector<GameObject*>& getChildren() const { return m_Children; }

	//�I�u�W�F�N�g�̔j��
	void destroy();
	//�j������Ă��邩�ǂ���
	bool isDestroy() const;

	void onCollisionEnterCallBack(GameObject* pHit);
	void onCollisionStayCallBack(GameObject* pHit);
	void onCollisionExitCallBack(GameObject* pHit);

	void onTriggerEnterCallBack(GameObject* pHit);
	void onTriggerStayCallBack(GameObject* pHit);
	void onTriggerExitCallBack(GameObject* pHit);

	IGameMediator* getGameMediator();

	Transform& getTransform();
	Action::ActionManager& getActionManager();

	//�R���|�[�l���g�̎擾
	template<typename T>
	T* getComponent();
	//�R���|�[�l���g�̒ǉ�
	template<typename T>
	T* addComponent();
	//�R���|�[�l���g�̍폜
	template<typename T>
	void removeComponent();

	const std::vector<AbstractComponent*>& getAllComponents();

	void setActive(bool value);
	bool isActive() const;

	//�^�O�̔�r
	inline bool compareTag(const std::string& tag) { return m_Tag == tag; }
	//�^�O�̐ݒ�
	inline void setTag(const std::string& tag) { m_Tag = tag; }
	//�^�O�̎擾
	inline const std::string& getTag() { return m_Tag; }

	//���O�̐ݒ�
	inline void setName(const std::string& name) { m_Name = name; }
	//���O�̎擾
	inline const std::string& getName() { return m_Name; }

	virtual void onPositionChanged(const Vec3& newPosition) {}
	virtual void onLocalPositionChanged(const Vec3& newLocalPosition) {}
	virtual void onScaleChanged(const Vec3& newScale) {}
	virtual void onLocalScaleChanged(const Vec3& newLocalScale) {}
	virtual void onAnglesChanged(const Vec3& newAngle) {}
	virtual void onLocalAnglesChanged(const Vec3& newLocalAngle) {}

	inline void setVisibleInEditor(bool isVisible) { m_IsVisibleInEditor = isVisible; };
	inline bool isVisibleInEditor() const { return m_IsVisibleInEditor; }

	inline void setSaveObject(bool isSaveObject) { m_IsSaveObject = isSaveObject; }
	inline bool isSaveObject() const { return m_IsSaveObject; }

	//�Ǘ��ԍ��̐ݒ�
	inline void setObjectNumber(int number) { m_ObjectNumber = number; }
	inline int getObjectNumber() const { return m_ObjectNumber; }

protected:
	virtual void start() {}
	virtual void update() {}
	virtual void onDestroy() {}

	virtual void onCollisionEnter(GameObject* pHit) {}
	virtual void onCollisionStay(GameObject* pHit) {}
	virtual void onCollisionExit(GameObject* pHit) {}

	virtual void onTriggerEnter(GameObject* pHit) {}
	virtual void onTriggerStay(GameObject* pHit) {}
	virtual void onTriggerExit(GameObject* pHit) {}

private:
	void objStart();
	void objOnDestroy();

protected:
	IGameMediator* m_pGameMediator;

private:
	bool m_Enabled;
	bool m_DestroyFlag;
	bool m_FirstUpdate;
	bool m_IsSaveObject;
	bool m_IsVisibleInEditor;
	int m_ObjectNumber;

	std::string m_Tag;
	std::string m_Name;

	ComponentManager* m_pComponentManager;

	//�e�I�u�W�F�N�g
	GameObject* m_pParent;
	//�q�I�u�W�F�N�g
	std::vector<GameObject*> m_Children;

	Transform* m_pTransform;
	Action::ActionManager* m_pActionManager;
};

template<typename T>
inline T * GameObject::getComponent()
{
	T* pComponent = m_pComponentManager->getComponent<T>();
	return pComponent;
}

template<typename T>
inline T * GameObject::addComponent()
{
	return m_pComponentManager->addComponent<T>();
}

template<typename T>
inline void GameObject::removeComponent()
{
	return m_pComponentManager->removeComponent<T>();
}
