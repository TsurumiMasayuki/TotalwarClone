#include "CircleColliderB2.h"
#include <box2d\box2d.h>
#include <Actor\Base\GameObject.h>

#include <Component\Box2D\PhysicsManagerB2.h>

#include <cassert>

void CircleColliderB2::setRadius(float radius)
{
#ifdef _DEBUG
	//1.0以下のサイズは安定性を損ねるのでassert
	assert(radius >= 1.0f);
#endif

	m_Radius = radius;
	init();
}

float CircleColliderB2::getRadius() const
{
	return m_Radius;
}

void CircleColliderB2::init()
{
	//既に登録されていたら削除
	if (m_pBody != nullptr)
		PhysicsManagerB2::getInstance()->destroyBody(m_pBody);

	Vec3 pos = getTransform().getLocalPosition();
	Vec3 scale = getTransform().getLocalScale();

	//X, Z座標を指定
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pos.x, pos.z);
	m_pBody = PhysicsManagerB2::getInstance()->createBody(bodyDef, this);

	//円の形状を指定
	b2CircleShape circleShape;
	circleShape.m_radius = m_Radius * scale.x;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = 100.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.isSensor = m_IsTrigger;

	m_pBody->CreateFixture(&fixtureDef);
}
