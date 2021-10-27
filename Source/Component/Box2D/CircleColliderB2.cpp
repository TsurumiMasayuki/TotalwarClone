#include "CircleColliderB2.h"
#include <box2d\box2d.h>
#include <Actor\Base\GameObject.h>

#include <Component\Box2D\PhysicsManagerB2.h>

#include <cassert>

CircleColliderB2::CircleColliderB2()
	: m_Radius(1.0f)
{
}

void CircleColliderB2::setRadius(float radius)
{
	m_Radius = radius;
	init();
}

float CircleColliderB2::getRadius() const
{
	return m_Radius;
}

void CircleColliderB2::init()
{
	//Šù‚É“o˜^‚³‚ê‚Ä‚¢‚½‚çíœ
	if (m_pBody != nullptr)
		PhysicsManagerB2::getInstance()->destroyBody(m_pBody);

	const Vec3& pos = getTransform().getLocalPosition();
	const Vec3& scale = getTransform().getLocalScale();

	//X, ZÀ•W‚ðŽw’è
	b2BodyDef bodyDef;
	bodyDef.type = m_BodyType;
	bodyDef.position.Set(pos.x, pos.z);
	bodyDef.fixedRotation = true;	//•¨—‹““®‚É‚æ‚é‰ñ“]‚ð§ŒÀ
	m_pBody = PhysicsManagerB2::getInstance()->createBody(bodyDef, this);

#ifdef _DEBUG
	if (m_Radius * scale.x < 1.0f) 
	{
		assert(!"1.0ˆÈ‰º‚ÌƒTƒCƒY‚ÍˆÀ’è«‚ª’á‰º‚µ‚Ü‚·");
	}
#endif

	//‰~‚ÌŒ`ó‚ðŽw’è
	b2CircleShape circleShape;
	circleShape.m_radius = m_Radius * scale.x;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = 100.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.isSensor = m_IsTrigger;

	m_pBody->CreateFixture(&fixtureDef);
}
