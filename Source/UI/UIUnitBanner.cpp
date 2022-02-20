#include "UIUnitBanner.h"
#include "Component\Graphics\SpriteRenderer.h"

#include "Unit\Unit.h"

#include "Math\MathUtility.h"

#include "Unit\UnitStats.h"
#include "Unit\UnitSelector.h"

void UIUnitBanner::onStart()
{

}

void UIUnitBanner::onUpdate()
{
	Camera* pCamera = getUser().getGameMediator()->getMainCamera();
	const Vec3& angles = pCamera->getTransform().getLocalAngles();

	m_pIconTransform->setLocalAngles(Vec3(0.0f, angles.y, 0.0f));
	m_pStickTransform->setLocalAngles(Vec3(0.0f, angles.y, 0.0f));
}

void UIUnitBanner::init(Unit* pSourceUnit)
{
	m_pSourceUnit = pSourceUnit;

	//アイコン用のオブジェクト生成
	auto pIconObject = new GameObject(getUser().getGameMediator());
	m_pIconTransform = &pIconObject->getTransform();
	m_pIconTransform->setLocalPosition(Vec3(0.0f, 100.0f, 0.0f));
	m_pIconTransform->setLocalScale(Vec3(25.0f, 25.0f, 1.0f));
	auto pIconSprite = pIconObject->addComponent<SpriteRenderer>();
	pIconSprite->setTextureByName("BoxFill");
	//チームによって色を変える
	pIconSprite->setColor(m_pSourceUnit->getTeamID() == 0 ? Color(DirectX::Colors::Orange) : Color(DirectX::Colors::Red));

	//アイコン下の棒用のオブジェクト生成
	auto pStickObject = new GameObject(getUser().getGameMediator());
	m_pStickTransform = &pStickObject->getTransform();
	m_pStickTransform->setLocalPosition(Vec3(0.0f, 50.0f, 0.0f));
	m_pStickTransform->setLocalScale(Vec3(2.5f, 100.0f, 0.0f));
	auto pStickSprite = pStickObject->addComponent<SpriteRenderer>();
	pStickSprite->setTextureByName("BoxFill");
	pStickSprite->setColor(Color(DirectX::Colors::DarkGray));

	//親子関係を設定
	pSourceUnit->getUser().addChild(*pStickObject);
	pSourceUnit->getUser().addChild(*pIconObject);
}
