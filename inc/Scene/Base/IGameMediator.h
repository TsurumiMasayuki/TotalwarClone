#pragma once

class GameObjectManager;
class IPhysicsManager;
class Camera;

class IGameMediator
{
public:
	virtual ~IGameMediator() {};

	virtual GameObjectManager& getGameObjectManager() = 0;
	virtual IPhysicsManager* getPhysicsManager() = 0;
	virtual Camera* getMainCamera() = 0;
	//virtual PhysicsWorld& getPhysicsWorld() = 0;

protected:
	IGameMediator() {};
};