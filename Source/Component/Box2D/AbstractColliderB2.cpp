#include "AbstractColliderB2.h"
#include "box2d\box2d.h"
#include "Actor\Base\GameObject.h"
#include "Math\MathUtility.h"

#include "Component\Box2D\PhysicsManagerB2.h"
#include "Unit\UnitObject.h"

AbstractColliderB2::AbstractColliderB2()
	: m_BodyType(b2_dynamicBody)
{
}

AbstractColliderB2::~AbstractColliderB2()
{
}

void AbstractColliderB2::onStart()
{
	m_pUnitObject = getUser().getComponent<UnitObject>();

	init();
}

void AbstractColliderB2::onUpdate()
{
	//トリガーなら反映しない
	if (m_IsTrigger) return;

	//2次元平面上の座標を3次元に反映
	getTransform().setLocalPosition(getPositionB2());

	//float angle = m_pBody->GetAngle();
	//getTransform().setLocalAngles(Vec3(0.0f, MathUtility::toDegree(angle), 0.0f));
}

void AbstractColliderB2::onDestroy()
{
	PhysicsManagerB2::getInstance()->destroyBody(m_pBody);
}

void AbstractColliderB2::setVelocity(float x, float z)
{
	if (m_pBody == nullptr) return;

	m_pBody->SetLinearVelocity(b2Vec2(x, z));
}

void AbstractColliderB2::setTrigger(bool isTrigger)
{
	m_IsTrigger = isTrigger;
	init();
}

bool AbstractColliderB2::getTrigger() const
{
	return m_IsTrigger;
}

void AbstractColliderB2::setBodyType(const b2BodyType& bodyType)
{
	m_BodyType = bodyType;
}

void AbstractColliderB2::onLocalPositionChanged(const Vec3& newPosition)
{
	if (!m_IsTrigger) return;

	setPositionB2(newPosition);
}

void AbstractColliderB2::onLocalScaleChanged(const Vec3& newScale)
{
	//一度作成したBodyのサイズ変更は出来ない為、初期化
	init();
}

void AbstractColliderB2::onEnable()
{
	m_pBody->SetEnabled(true);
}

void AbstractColliderB2::onDisable()
{
	m_pBody->SetEnabled(false);
}

void AbstractColliderB2::onCollisionEnter(AbstractColliderB2* pHit)
{
	getUser().onCollisionEnterCallBack(&pHit->getUser());

	//自身と当たったオブジェクトどちらもUnitObjectと併用されているなら専用コールバックを呼ぶ
	if (m_pUnitObject != nullptr && pHit->m_pUnitObject != nullptr)
		m_pUnitObject->onCollisionEnter(pHit->m_pUnitObject);
}

void AbstractColliderB2::onCollisionStay(AbstractColliderB2* pHit)
{
	getUser().onCollisionStayCallBack(&pHit->getUser());

	//自身と当たったオブジェクトどちらもUnitObjectと併用されているなら専用コールバックを呼ぶ
	if (m_pUnitObject != nullptr && pHit->m_pUnitObject != nullptr)
		m_pUnitObject->onCollisionStay(pHit->m_pUnitObject);
}

void AbstractColliderB2::onCollisionExit(AbstractColliderB2* pHit)
{
	getUser().onCollisionExitCallBack(&pHit->getUser());

	//自身と当たったオブジェクトどちらもUnitObjectと併用されているなら専用コールバックを呼ぶ
	if (m_pUnitObject != nullptr && pHit->m_pUnitObject != nullptr)
		m_pUnitObject->onCollisionExit(pHit->m_pUnitObject);
}

void AbstractColliderB2::onTriggerEnter(AbstractColliderB2* pHit)
{
	getUser().onTriggerEnterCallBack(&pHit->getUser());

	//自身と当たったオブジェクトどちらもUnitObjectと併用されているなら専用コールバックを呼ぶ
	if (m_pUnitObject != nullptr && pHit->m_pUnitObject != nullptr)
		m_pUnitObject->onTriggerEnter(pHit->m_pUnitObject);
}

void AbstractColliderB2::onTriggerStay(AbstractColliderB2* pHit)
{
	getUser().onTriggerStayCallBack(&pHit->getUser());

	//自身と当たったオブジェクトどちらもUnitObjectと併用されているなら専用コールバックを呼ぶ
	if (m_pUnitObject != nullptr && pHit->m_pUnitObject != nullptr)
		m_pUnitObject->onTriggerStay(pHit->m_pUnitObject);
}

void AbstractColliderB2::onTriggerExit(AbstractColliderB2* pHit)
{
	getUser().onTriggerExitCallBack(&pHit->getUser());

	//自身と当たったオブジェクトどちらもUnitObjectと併用されているなら専用コールバックを呼ぶ
	if (m_pUnitObject != nullptr && pHit->m_pUnitObject != nullptr)
		m_pUnitObject->onTriggerExit(pHit->m_pUnitObject);
}

void AbstractColliderB2::setPositionB2(const Vec3& position)
{
	float angle = m_pBody->GetAngle();
	m_pBody->SetTransform(b2Vec2(position.x, position.z), angle);
}

Vec3 AbstractColliderB2::getPositionB2() const
{
	const b2Vec2& position = m_pBody->GetPosition();
	return Vec3(position.x, 0.0f, position.y);
}
