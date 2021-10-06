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

	//親オブジェクトを設定(nullptrで解除)
	void setParent(GameObject* parent);
	//親オブジェクトを取得
	GameObject* getParent();

	//子オブジェクトを登録
	void addChild(GameObject& child);
	//子オブジェクトを登録解除
	void removeChild(GameObject& child);
	//子オブジェクトを全取得
	const std::vector<GameObject*>& getChildren() const { return m_Children; }

	//オブジェクトの破棄
	void destroy();
	//破棄されているかどうか
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

	//コンポーネントの取得
	template<typename T>
	T* getComponent();
	//コンポーネントの追加
	template<typename T>
	T* addComponent();
	//コンポーネントの削除
	template<typename T>
	void removeComponent();

	const std::vector<AbstractComponent*>& getAllComponents();

	void setActive(bool value);
	bool isActive() const;

	//タグの比較
	inline bool compareTag(const std::string& tag) { return m_Tag == tag; }
	//タグの設定
	inline void setTag(const std::string& tag) { m_Tag = tag; }
	//タグの取得
	inline const std::string& getTag() { return m_Tag; }

	//名前の設定
	inline void setName(const std::string& name) { m_Name = name; }
	//名前の取得
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

	//管理番号の設定
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

	//親オブジェクト
	GameObject* m_pParent;
	//子オブジェクト
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
