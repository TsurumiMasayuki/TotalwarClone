#include "PhysicsManagerB2.h"
#include <cassert>
#include "Device\GameDevice.h"

PhysicsManagerB2* PhysicsManagerB2::pInstance = nullptr;

PhysicsManagerB2::PhysicsManagerB2()
	: m_World(b2Vec2_zero)
{
	assert(pInstance == nullptr);

	pInstance = this;

	m_World.SetContactListener(&m_ContactListner);
}

PhysicsManagerB2::~PhysicsManagerB2()
{
	pInstance = nullptr;
}

void PhysicsManagerB2::onStart()
{
	m_World.SetAllowSleeping(false);
}

void PhysicsManagerB2::onUpdate()
{
	float deltaTime = GameDevice::getGameTime().getDeltaTime();
	m_World.Step(deltaTime, 6, 2);

	m_ContactListner.update();
}

b2Body* PhysicsManagerB2::createBody(const b2BodyDef& bodyDef, AbstractColliderB2* pCollider)
{
	b2Body* pBody = m_World.CreateBody(&bodyDef);
	m_ContactListner.addPair(pBody, pCollider);
	return pBody;
}

void PhysicsManagerB2::destroyBody(b2Body* pBody)
{
	m_ContactListner.removePair(pBody);
	m_World.DestroyBody(pBody);
}

PhysicsManagerB2* PhysicsManagerB2::getInstance()
{
	return pInstance;
}
