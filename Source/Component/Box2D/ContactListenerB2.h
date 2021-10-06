#pragma once
#include <functional>
#include <unordered_map>
#include <list>
#include <box2d\box2d.h>

class AbstractColliderB2;

class ContactListenerB2
	: public b2ContactListener
{
private:
	class ContactInfo
	{
	public:
		//b2Contactから情報をコピー
		void copy(b2Contact* pOrigin);

	public:
		b2Contact* m_pOrigin;
		b2Fixture* m_pFixtureA;
		b2Fixture* m_pFixtureB;
	};

public:
	ContactListenerB2();
	virtual ~ContactListenerB2();

	virtual void BeginContact(b2Contact* contact) override;
	virtual void EndContact(b2Contact* contact) override;

	void addPair(b2Body* pBody, AbstractColliderB2* pCollider);
	void removePair(b2Body* pBody);

	void update();

private:
	void loopCallback(const std::list<ContactInfo*>& list,
		const std::function<void(AbstractColliderB2*, AbstractColliderB2*)>& collisionFunc,
		const std::function<void(AbstractColliderB2*, AbstractColliderB2*)>& triggerFunc);

private:
	std::unordered_map<b2Body*, AbstractColliderB2*> m_Pairs;

	std::list<ContactInfo*> m_CollisionEnterList;
	std::list<ContactInfo*> m_CollisionStayList;
	std::list<ContactInfo*> m_CollisionExitList;

	//コリジョン系関数
	std::function<void(AbstractColliderB2*, AbstractColliderB2*)> m_CollisionEnter;
	std::function<void(AbstractColliderB2*, AbstractColliderB2*)> m_CollisionStay;
	std::function<void(AbstractColliderB2*, AbstractColliderB2*)> m_CollisionExit;

	//トリガー系関数
	std::function<void(AbstractColliderB2*, AbstractColliderB2*)> m_TriggerEnter;
	std::function<void(AbstractColliderB2*, AbstractColliderB2*)> m_TriggerStay;
	std::function<void(AbstractColliderB2*, AbstractColliderB2*)> m_TriggerExit;
};