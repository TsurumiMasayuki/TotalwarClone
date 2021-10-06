#pragma once
#include <vector>
#include <Component\Base\AbstractComponent.h>
#include <Component\Physics\CollisionHit.h>

class btTransform;
class btCollisionShape;
class btRigidBody;

class AbstractColliderBt
	: public AbstractComponent
{
public:
	AbstractColliderBt();
	virtual ~AbstractColliderBt();

	virtual void onStart() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;

	virtual void createCollisionShape() = 0;

	virtual void onLocalPositionChanged(const Vec3& newLocalPosition) override;
	virtual void onLocalScaleChanged(const Vec3& newLocalScale) override;
	virtual void onLocalAnglesChanged(const Vec3& newLocalAngle) override;

	void applyForce(const Vec3& force);
	void applyForceImpluse(const Vec3& force);
	void applyTorque(const Vec3& torque);
	void applyTorqueImpluse(const Vec3& torque);
	void setMass(const float& mass);
	const float& getMass() const;

	btRigidBody* getRigidBody();

	void onCollision(AbstractColliderBt* pHit);
	void clearHitStates();
	void checkHitStates();

	void setTrigger(bool trigger);

private:
	void initialize();

	void onCollisionEnter(AbstractColliderBt* pHit);
	void onCollisionStay(AbstractColliderBt* pHit);
	void onCollisionExit(AbstractColliderBt* pHit);

	void onTriggerEnter(AbstractColliderBt* pHit);
	void onTriggerStay(AbstractColliderBt* pHit);
	void onTriggerExit(AbstractColliderBt* pHit);

	CollisionHit<AbstractColliderBt>* getCollHit();

protected:
	btCollisionShape* m_pCollisionShape;
	btRigidBody* m_pRigidBody;

private:
	CollisionHit<AbstractColliderBt>* m_pCollHit;

	std::vector<CollisionHit<AbstractColliderBt>*> m_PreviousHits;
	std::vector<CollisionHit<AbstractColliderBt>*> m_CurrentHits;

	float m_Mass;
	bool m_Initialized;
	bool m_IsTrigger;
};