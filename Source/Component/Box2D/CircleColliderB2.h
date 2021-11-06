#pragma once
#include "Component\Box2D\AbstractColliderB2.h"

class CircleColliderB2
	: public AbstractColliderB2
{
public:
	CircleColliderB2();

	virtual void init() override;

	void setRadius(float radius);
	float getRadius() const;

private:
	float m_Radius;
};