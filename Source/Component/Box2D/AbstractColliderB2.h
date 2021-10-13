#pragma once
#include <Component\Base\AbstractComponent.h>
#include "box2d\b2_body.h"

class UnitObject;

class AbstractColliderB2
	: public AbstractComponent
{
public:
	AbstractColliderB2();
	~AbstractColliderB2();

	virtual void onStart() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;

	virtual void init() = 0;

	void setVelocity(float x, float z);

	void setTrigger(bool isTrigger);
	bool getTrigger() const;

	void setBodyType(const b2BodyType& bodyType);

	virtual void onLocalPositionChanged(const Vec3& newPosition) override;
	virtual void onLocalScaleChanged(const Vec3& newScale) override;

	virtual void onEnable() override;
	virtual void onDisable() override;

	void onCollisionEnter(AbstractColliderB2* pHit);
	void onCollisionStay(AbstractColliderB2* pHit);
	void onCollisionExit(AbstractColliderB2* pHit);

	void onTriggerEnter(AbstractColliderB2* pHit);
	void onTriggerStay(AbstractColliderB2* pHit);
	void onTriggerExit(AbstractColliderB2* pHit);

protected:
	void setPositionB2(const Vec3& position);
	Vec3 getPositionB2() const;

protected:
	bool m_IsTrigger;
	b2Body* m_pBody;
	UnitObject* m_pUnitObject;
	b2BodyType m_BodyType;
};