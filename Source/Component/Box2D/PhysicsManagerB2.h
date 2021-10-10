#pragma once
#include "Component\Base\AbstractComponent.h"
#include "Component\Box2D\ContactListenerB2.h"

class AbstactColliderB2;

class PhysicsManagerB2
	: public AbstractComponent
{
public:
	PhysicsManagerB2();
	~PhysicsManagerB2();

	virtual void onStart() override;
	virtual void onUpdate() override;

	b2Body* createBody(const b2BodyDef& bodyDef, AbstractColliderB2* pCollider);
	void destroyBody(b2Body* pBody);

	static PhysicsManagerB2* getInstance();

private:
	static PhysicsManagerB2* pInstance;

	b2World m_World;
	ContactListenerB2 m_ContactListner;
};