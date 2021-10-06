#include "ContactListenerB2.h"
#include <iterator>
#include <algorithm>
#include "Component\Box2D\AbstractColliderB2.h"

ContactListenerB2::ContactListenerB2()
{
	m_CollisionEnter = [](AbstractColliderB2* pColl, AbstractColliderB2* pHit)
	{
		pColl->onCollisionEnter(pHit);
	};

	m_CollisionStay = [](AbstractColliderB2* pColl, AbstractColliderB2* pHit)
	{
		pColl->onCollisionStay(pHit);
	};

	m_CollisionExit = [](AbstractColliderB2* pColl, AbstractColliderB2* pHit)
	{
		pColl->onCollisionExit(pHit);
	};

	m_TriggerEnter = [](AbstractColliderB2* pColl, AbstractColliderB2* pHit)
	{
		pColl->onTriggerEnter(pHit);
	};

	m_TriggerStay = [](AbstractColliderB2* pColl, AbstractColliderB2* pHit)
	{
		pColl->onTriggerStay(pHit);
	};

	m_TriggerExit = [](AbstractColliderB2* pColl, AbstractColliderB2* pHit)
	{
		pColl->onTriggerExit(pHit);
	};
}

ContactListenerB2::~ContactListenerB2()
{
	//中身を全削除
	for (auto pContactInfo : m_CollisionEnterList)
	{
		delete pContactInfo;
	}

	for (auto pContactInfo : m_CollisionStayList)
	{
		delete pContactInfo;
	}

	for (auto pContactInfo : m_CollisionExitList)
	{
		delete pContactInfo;
	}
}

void ContactListenerB2::BeginContact(b2Contact* contact)
{
	auto pContactInfo = new ContactInfo();
	pContactInfo->copy(contact);
	m_CollisionEnterList.emplace_back(pContactInfo);
}

void ContactListenerB2::EndContact(b2Contact* contact)
{
	auto find = std::find_if(m_CollisionStayList.begin(), m_CollisionStayList.end(), [contact](ContactInfo* pContact)
		{
			return pContact->m_pOrigin == contact;
		}
	);

	m_CollisionExitList.emplace_back(*find);
	//イテレータ破壊が起こるため後で削除
	m_CollisionStayList.erase(find);
}

void ContactListenerB2::addPair(b2Body* pBody, AbstractColliderB2* pCollider)
{
	m_Pairs.emplace(pBody, pCollider);
}

void ContactListenerB2::removePair(b2Body* pBody)
{
	m_Pairs.erase(pBody);
}

void ContactListenerB2::update()
{
	//CollisionEnter
	loopCallback(m_CollisionEnterList, m_CollisionEnter, m_TriggerEnter);

	//CollisionStay
	loopCallback(m_CollisionStayList, m_CollisionStay, m_TriggerStay);

	//CollisionExit
	loopCallback(m_CollisionExitList, m_CollisionExit, m_TriggerExit);

	//CollisionEnterのリストをCollisionStayのリストにコピー
	std::copy(m_CollisionEnterList.begin(), m_CollisionEnterList.end(), std::back_inserter(m_CollisionStayList));

	//CollisionExitの中身を全削除
	for (auto pContactInfo : m_CollisionExitList)
	{
		delete pContactInfo;
	}

	//クリア
	m_CollisionEnterList.clear();
	m_CollisionExitList.clear();
}

void ContactListenerB2::loopCallback(const std::list<ContactInfo*>& list,
	const std::function<void(AbstractColliderB2*, AbstractColliderB2*)>& collisionFunc,
	const std::function<void(AbstractColliderB2*, AbstractColliderB2*)>& triggerFunc)
{
	for (auto pContact : list)
	{
		auto pColliderA = m_Pairs.at(pContact->m_pFixtureA->GetBody());
		auto pColliderB = m_Pairs.at(pContact->m_pFixtureB->GetBody());

		//トリガーの状態で分岐
		if (pColliderA->getTrigger())
			triggerFunc(pColliderA, pColliderB);
		else
			collisionFunc(pColliderA, pColliderB);

		//トリガーの状態で分岐
		if (pColliderB->getTrigger())
			triggerFunc(pColliderB, pColliderA);
		else
			collisionFunc(pColliderB, pColliderA);
	}
}

void ContactListenerB2::ContactInfo::copy(b2Contact* pOrigin)
{
	m_pOrigin = pOrigin;
	m_pFixtureA = pOrigin->GetFixtureA();
	m_pFixtureB = pOrigin->GetFixtureB();
}
