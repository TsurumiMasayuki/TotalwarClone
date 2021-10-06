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

	//�I�u�W�F�N�g��j��
	void destroy();
	//�L����/��������ݒ�
	void setActive(bool value);
	//�L����/���������擾
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

	//�^�O�̔�r
	bool compareTag(const std::string& tag);
	//�^�O�̐ݒ�
	void setTag(const std::string& tag);
	//�^�O�̎擾
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

