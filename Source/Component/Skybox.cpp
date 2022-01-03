#include "Skybox.h"
#include "Actor\Base\GameObject.h"
#include "Component\Graphics\SpriteRenderer.h"

void Skybox::onStart()
{
	//���ʂ̃X�v���C�g
	auto pFrontObject = new GameObject(getUser().getGameMediator());
	auto pFrontSprite = pFrontObject->addComponent<SpriteRenderer>();

	//�w�ʂ̃X�v���C�g
	auto pBackObject = new GameObject(getUser().getGameMediator());
	auto pBackSprite = pBackObject->addComponent<SpriteRenderer>();

	//�E�̃X�v���C�g
	auto pRightObject = new GameObject(getUser().getGameMediator());
	auto pRightSprite = pRightObject->addComponent<SpriteRenderer>();

	//���̃X�v���C�g
	auto pLeftObject = new GameObject(getUser().getGameMediator());
	auto pLeftSprite = pLeftObject->addComponent<SpriteRenderer>();

	//��̃X�v���C�g
	auto pTopObject = new GameObject(getUser().getGameMediator());
	auto pTopSprite = pTopObject->addComponent<SpriteRenderer>();

	//���̃X�v���C�g
	auto pDownObject = new GameObject(getUser().getGameMediator());
	auto pDownSprite = pDownObject->addComponent<SpriteRenderer>();

	//���W�ݒ�
	pFrontObject->getTransform().setLocalPosition(Vec3(0.0f, 0.0f, 2500.0f));
	pBackObject->getTransform().setLocalPosition(Vec3(0.0f, 0.0f, -2500.0f));
	pRightObject->getTransform().setLocalPosition(Vec3(2500.0f, 0.0f, 0.0f));
	pLeftObject->getTransform().setLocalPosition(Vec3(-2500.0f, 0.0f, 0.0f));
	pTopObject->getTransform().setLocalPosition(Vec3(0.0f, 2500.0f, 0.0f));
	pDownObject->getTransform().setLocalPosition(Vec3(0.0f, -2500.0f, 0.0f));

	//�X�P�[���ݒ�
	pFrontObject->getTransform().setLocalScale(Vec3(5000.0f, 5000.0f, 1.0f));
	pBackObject->getTransform().setLocalScale(Vec3(5000.0f, 5000.0f, 1.0f));
	pRightObject->getTransform().setLocalScale(Vec3(5000.0f, 5000.0f, 1.0f));
	pLeftObject->getTransform().setLocalScale(Vec3(5000.0f, 5000.0f, 1.0f));
	pTopObject->getTransform().setLocalScale(Vec3(5000.0f, 5000.0f, 1.0f));
	pDownObject->getTransform().setLocalScale(Vec3(5000.0f, 5000.0f, 1.0f));

	//��]�ݒ�
	pFrontObject->getTransform().setLocalAngles(Vec3(0.0f, 180.0f, 0.0f));
	pBackObject->getTransform().setLocalAngles(Vec3(0.0f, 0.0f, 0.0f));
	pRightObject->getTransform().setLocalAngles(Vec3(0.0f, -90.0f, 0.0f));
	pLeftObject->getTransform().setLocalAngles(Vec3(0.0f, 90.0f, 0.0f));
	pTopObject->getTransform().setLocalAngles(Vec3(90.0f, 180.0f, 0.0f));
	pDownObject->getTransform().setLocalAngles(Vec3(-90.0f, 0.0f, 0.0f));

	//�X�v���C�g�ݒ�
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
