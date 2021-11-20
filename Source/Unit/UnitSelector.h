#pragma once
#include "Component\Base\AbstractComponent.h"
#include "Component\Graphics\InstancedRenderer.h"
#include "Utility\Timer.h"

#include "Unit\ObjectPlacement.h"

class Cursor;
class Unit;
class CircleColliderB2;
class ValueMapMaterial;

struct PreviewObjInstance
{
	DirectX::XMFLOAT4X4 instanceMat;
	DirectX::XMFLOAT4 instanceColor;
};

class UnitSelector
	: public AbstractComponent
{
public:
	virtual void onStart() override;
	virtual void onUpdate() override;

	virtual void onTriggerEnter(GameObject* pHit) override;
	virtual void onTriggerExit(GameObject* pHit) override;

	void init(Cursor* pCursor, int teamID, ValueMapMaterial* pMaterial);
	void selectUnit(Unit* pUnit);

private:
	//ユニット選択の更新
	void updateUnitSelecting();
	//ユニット配置の更新
	void updateUnitPlacement();

	void setInstanceInfo(const Vec3& placePos, float angle, int width);

private:
	Cursor* m_pCursor;
	int m_TeamID;
	Unit* m_pTargetUnit;
	Unit* m_pSelectedUnit;
	Unit* m_pAttackTargetUnit;
	CircleColliderB2* m_pCollider;

	GameObject* m_pAttackDisplayObj;

	Vec3 m_UnitPlacePosBegin;

	//入力のインターバル
	Timer m_InputInterval;

	//配置予測用のオブジェクト配置補助
	ObjectPlacement m_ObjPlacement;

	//配置予測用のオブジェクト
	std::vector<GameObject*> m_PreviewObjects;
	//配置予測用のInstancedRenderer
	InstancedRenderer<PreviewObjInstance>* m_pPreviewObjRenderer;
};