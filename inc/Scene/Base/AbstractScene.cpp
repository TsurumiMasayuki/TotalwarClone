#include "AbstractScene.h"
#include <Actor\Base\GameObject.h>
#include <Device\GameDevice.h>

#include <fstream>

GameObjectManager & AbstractScene::getGameObjectManager()
{
	return m_GameObjectManager;
}

IPhysicsManager * AbstractScene::getPhysicsManager()
{
	return &m_PhysicsManager;
}

Camera * AbstractScene::getMainCamera()
{
	return m_pDefaultCamera;
}

void AbstractScene::draw()
{
	GameDevice::getRenderManager().setDefaultRenderTarget();
	GameDevice::getRenderManager().draw();
	GameDevice::getRenderManager().present();
}

void AbstractScene::baseStart()
{
	//デフォルトカメラを生成
	auto pObj = new GameObject(this);
	pObj->setName("MainCamera");
	pObj->setSaveObject(false);
	m_pDefaultCamera = pObj->addComponent<Camera>();
	m_pDefaultCamera->setDrawMode(DrawMode::DrawMode_3D);
	pObj->getTransform().setLocalPosition(Vec3(0, 0, -10));

	m_PhysicsManager.start();

	start();
}

void AbstractScene::baseUpdate()
{
	m_GameObjectManager.update();
	update();
	m_PhysicsManager.update();
}

void AbstractScene::baseShutdown()
{
	shutdown();
	m_PhysicsManager.shutdown();
	m_GameObjectManager.shutdown();
}