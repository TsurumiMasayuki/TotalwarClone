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
	//���j�b�g�I���̍X�V
	void updateUnitSelecting();
	//���j�b�g�z�u�̍X�V
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

	//���͂̃C���^�[�o��
	Timer m_InputInterval;

	//�z�u�\���p�̃I�u�W�F�N�g�z�u�⏕
	ObjectPlacement m_ObjPlacement;

	//�z�u�\���p�̃I�u�W�F�N�g
	std::vector<GameObject*> m_PreviewObjects;
	//�z�u�\���p��InstancedRenderer
	InstancedRenderer<PreviewObjInstance>* m_pPreviewObjRenderer;
};