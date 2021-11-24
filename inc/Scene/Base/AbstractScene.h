#pragma once
#include <string>
#include <Actor\Base\GameObjectManager.h>
#include <Component\Graphics\Camera.h>
#include <Physics\PhysicsManagerBt.h>
#include <Scene\Base\IGameMediator.h>
#include <Utility\INonCopyable.h>

//シーン抽象クラス
class AbstractScene
	: public IGameMediator, private INonCopyable
{
public:
	AbstractScene() : m_PhysicsManager() {};
	virtual ~AbstractScene() {};

	void baseStart();
	void baseUpdate();
	void baseShutdown();
	virtual void draw();

	virtual void lateUpdate() {};

	virtual std::string nextScene() = 0;
	virtual bool isEnd() = 0;

	virtual GameObjectManager & getGameObjectManager() override;
	virtual IPhysicsManager * getPhysicsManager() override;
	virtual Camera* getMainCamera() override;

protected:
	virtual void start() = 0;
	virtual void update() = 0;
	virtual void shutdown() = 0;

protected:
	Camera* m_pDefaultCamera;

private:
	GameObjectManager m_GameObjectManager;
	PhysicsManagerBt m_PhysicsManager;
};

