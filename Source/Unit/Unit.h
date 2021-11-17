#pragma once
#include <vector>
#include <unordered_map>
#include "Component\Base\AbstractComponent.h"
#include "Utility\Timer.h"

#include "Blockbench/BlockbenchModel.h"
#include "Unit\ObjectPlacement.h"

struct UnitStats;
class IPlayer;
class ValueMap;
class UnitObject;
class UnitRenderHelper;
class EffectRenderHelper;

//���j�b�g�N���X
class Unit
	: public AbstractComponent
{
public:
	virtual void onStart() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;

	virtual void onEnable() override;
	virtual void onDisable() override;

	void init(IPlayer* pPlayer, const UnitStats* pUnitStats, ValueMap* pValueMap, UnitRenderHelper* pUnitRenderHelper, EffectRenderHelper* pEffectRenderHelper);
	void setPosition(const Vec3& position, float angle, int unitWidth);
	void setDestination(const Vec3& destination, float angle, int unitWidth, bool isMoveCommand = true);

	float getSpacePerObject() const;
	float getAngle() const;
	int getWidth() const;

	void setTarget(Unit* pTarget);
	Unit* getTarget();

	float getHealth() const;
	float getShield() const;
	int getObjectCount() const;
	const UnitStats* getUnitStats() const;

	int getTeamID() const;

	//�X�e�[�g�ڍs���b�N�����ǂ���
	bool isStateLocked();

	void onEnterCombat(Unit* pEnemyUnit);
	//�퓬�̒��~����������
	void forceEscapeCombat();
	//�퓬�������邩�ݒ�
	void setAllowCombat(bool value);
	//�퓬�������邩�ǂ���
	bool isAllowCombat() const;
	bool isInCombat() const;

private:
	void updateCenterPosition();

private:
	std::vector<GameObject*> m_GameObjects;
	std::vector<UnitObject*> m_UnitObjects;

	IPlayer* m_pOwnerPlayer;

	Unit* m_pTargetUnit;

	int m_TeamID;
	bool m_IsAllowCombat;

	Timer m_StateLockTimer;

	ValueMap* m_pValueMap;
	const UnitStats* m_pUnitStats;
	UnitRenderHelper* m_pUnitRenderHelper;

	ObjectPlacement m_ObjectPlacement;
};