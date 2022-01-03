#include "Skybox.h"
#include "Actor\Base\GameObject.h"
#include "Component\Graphics\SpriteRenderer.h"

void Skybox::onStart()
{
	//正面のスプライト
	auto pFrontObject = new GameObject(getUser().getGameMediator());
	auto pFrontSprite = pFrontObject->addComponent<SpriteRenderer>();

	//背面のスプライト
	auto pBackObject = new GameObject(getUser().getGameMediator());
	auto pBackSprite = pBackObject->addComponent<SpriteRenderer>();

	//右のスプライト
	auto pRightObject = new GameObject(getUser().getGameMediator());
	auto pRightSprite = pRightObject->addComponent<SpriteRenderer>();

	//左のスプライト
	auto pLeftObject = new GameObject(getUser().getGameMediator());
	auto pLeftSprite = pLeftObject->addComponent<SpriteRenderer>();

	//上のスプライト
	auto pTopObject = new GameObject(getUser().getGameMediator());
	auto pTopSprite = pTopObject->addComponent<SpriteRenderer>();

	//下のスプライト
	auto pDownObject = new GameObject(getUser().getGameMediator());
	auto pDownSprite = pDownObject->addComponent<SpriteRenderer>();

	//座標設定
	pFrontObject->getTransform().setLocalPosition(Vec3(0.0f, 0.0f, 2500.0f));
	pBackObject->getTransform().setLocalPosition(Vec3(0.0f, 0.0f, -2500.0f));
	pRightObject->getTransform().setLocalPosition(Vec3(2500.0f, 0.0f, 0.0f));
	pLeftObject->getTransform().setLocalPosition(Vec3(-2500.0f, 0.0f, 0.0f));
	pTopObject->getTransform().setLocalPosition(Vec3(0.0f, 2500.0f, 0.0f));
	pDownObject->getTransform().setLocalPosition(Vec3(0.0f, -2500.0f, 0.0f));

	//スケール設定
	pFrontObject->getTransform().setLocalScale(Vec3(5000.0f, 5000.0f, 1.0f));
	pBackObject->getTransform().setLocalScale(Vec3(5000.0f, 5000.0f, 1.0f));
	pRightObject->getTransform().setLocalScale(Vec3(5000.0f, 5000.0f, 1.0f));
	pLeftObject->getTransform().setLocalScale(Vec3(5000.0f, 5000.0f, 1.0f));
	pTopObject->getTransform().setLocalScale(Vec3(5000.0f, 5000.0f, 1.0f));
	pDownObject->getTransform().setLocalScale(Vec3(5000.0f, 5000.0f, 1.0f));

	//回転設定
	pFrontObject->getTransform().setLocalAngles(Vec3(0.0f, 180.0f, 0.0f));
	pBackObject->getTransform().setLocalAngles(Vec3(0.0f, 0.0f, 0.0f));
	pRightObject->getTransform().setLocalAngles(Vec3(0.0f, -90.0f, 0.0f));
	pLeftObject->getTransform().setLocalAngles(Vec3(0.0f, 90.0f, 0.0f));
	pTopObject->getTransform().setLocalAngles(Vec3(90.0f, 180.0f, 0.0f));
	pDownObject->getTransform().setLocalAngles(Vec3(-90.0f, 0.0f, 0.0f));

	//スプライト設定
	pFrontSprite->setTextureByName("SkyboxFront");
	pBackSprite->setTextureByName("SkyboxBack");
	pRightSprite->setTextureByName("SkyboxRight");
	pLeftSprite->setTextureByName("SkyboxLeft");
	pTopSprite->setTextureByName("SkyboxUp");
	pDownSprite->setTextureByName("SkyboxDown");
}

void Skybox::onUpdate()
{
}
