#pragma once
#include <vector>
#include <Math\Vec3.h>

class GameObject;

struct RayHitResult
{
	RayHitResult()
		: pHitObject(nullptr),
		hitPoint(Vec3::zero())
	{}
	RayHitResult(GameObject* pHitObject, Vec3 hitPoint)
		: pHitObject(pHitObject),
		hitPoint(hitPoint)
	{}

	GameObject* pHitObject;
	Vec3 hitPoint;
};

class IPhysicsManager
{
public:
	virtual ~IPhysicsManager() {}

	virtual void start() = 0;
	virtual void update() = 0;
	virtual void shutdown() = 0;

	virtual bool raycastSingle(const Vec3& from, const Vec3& to, RayHitResult& rayResult) = 0;
	virtual bool raycastAll(const Vec3& from, const Vec3& to, std::vector<RayHitResult>& rayResult) = 0;

protected:
	IPhysicsManager() {}
};