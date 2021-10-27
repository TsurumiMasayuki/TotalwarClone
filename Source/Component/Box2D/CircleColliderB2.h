#pragma once
#include "Component\Box2D\AbstractColliderB2.h"

class CircleColliderB2
	: public AbstractColliderB2
{
public:
	CircleColliderB2();

	void setRadius(float radius);
	float getRadius() const;

private:
	virtual void init() override;

private:
	float m_Radius;
};