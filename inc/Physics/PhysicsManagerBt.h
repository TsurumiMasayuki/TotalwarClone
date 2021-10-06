#pragma once
#include <unordered_map>
#include <btBulletDynamicsCommon.h>
#include <Physics\IPhysicsManager.h>

class AbstractColliderBt;

class PhysicsManagerBt : public IPhysicsManager
{
public:
	virtual void start() override;
	virtual void update() override;
	virtual void shutdown() override;
	virtual bool raycastSingle(const Vec3& from, const Vec3& to, RayHitResult& rayResult);
	virtual bool raycastAll(const Vec3& from, const Vec3& to, std::vector<RayHitResult>& rayResult);

	void addCollider(AbstractColliderBt* pCollider);
	void removeCollider(AbstractColliderBt* pCollider);

private:
	btDefaultCollisionConfiguration* m_pCollisionConfig;
	btCollisionDispatcher* m_pCollisionDispatcher;
	btBroadphaseInterface* m_pOverlappingPairCache;
	btSequentialImpulseConstraintSolver* m_pSolver;
	btDiscreteDynamicsWorld* m_pDynamicsWorld;

	std::unordered_map<const btCollisionObject*, AbstractColliderBt*> m_COCollPairs;
};