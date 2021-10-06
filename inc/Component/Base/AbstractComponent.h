#pragma once
#include <string>
#include <Component\Utility\ITransformEventListner.h>
#include <Utility\INonCopyable.h>

#include <Math\Vec3.h>

class GameObject;
class Transform;

class AbstractComponent
	: private INonCopyable, public ITransformEventListner
{
public:
	virtual ~AbstractComponent();

	//オブジェクトを破棄
	void destroy();
	//有効化/無効化を設定
	void setActive(bool value);
	//有効化/無効化を取得
	bool isActive() const;

	void update();

	virtual void onStart() = 0;
	virtual void onUpdate() = 0;

	virtual void onDestroy() {};
	virtual void onEnable() {};
	virtual void onDisable() {};

	virtual void onCollisionEnter(GameObject* pHit) {};
	virtual void onCollisionStay(GameObject* pHit) {};
	virtual void onCollisionExit(GameObject* pHit) {};

	virtual void onTriggerEnter(GameObject* pHit) {};
	virtual void onTriggerStay(GameObject* pHit) {};
	virtual void onTriggerExit(GameObject* pHit) {};

	int getUpdateOrder() const;

	void setUser(GameObject& user);
	GameObject& getUser();

	Transform& getTransform();

	//タグの比較
	bool compareTag(const std::string& tag);
	//タグの設定
	void setTag(const std::string& tag);
	//タグの取得
	const std::string& getTag();

	virtual void onLocalPositionChanged(const Vec3& newLocalPosition) {}
	virtual void onLocalScaleChanged(const Vec3& newLocalScale) {}
	virtual void onLocalAnglesChanged(const Vec3& newLocalAngle) {}

	virtual bool isUsableInEditor() { return false; }

protected:
	AbstractComponent(int updateOrder = 100);

protected:
	GameObject* m_pUser;
	int m_UpdateOrder;
	bool m_Enabled;
	bool m_FirstUpdate;
};

