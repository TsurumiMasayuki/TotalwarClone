#pragma once
#include <vector>
#include <unordered_set>
#include "Component\Base\AbstractComponent.h"
#include "Utility\Timer.h"

class Unit;
class Attack;
class CircleColliderB2;
class ValueMap;
class TestEffect_Beam;
class CubeTrailEffect;
class EffectRenderHelper;

class UnitObject
	: public AbstractComponent
{
public:
	virtual void onStart() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;

	void init(Unit* pUnit, ValueMap* pValueMap, EffectRenderHelper* pEffectRenderHelper);

	//�ړI�n��ݒ�
	void setDestination(const Vec3& destination, bool isMoveCommand = true);

	//�R���C�_�[�����Z�b�g
	void resetCollider();

	void onCollisionEnter(UnitObject* pUnitObject);
	void onCollisionStay(UnitObject* pUnitObject);
	void onCollisionExit(UnitObject* pUnitObject);

	void onTriggerEnter(UnitObject* pUnitObject);
	void onTriggerStay(UnitObject* pUnitObject);
	void onTriggerExit(UnitObject* pUnitObject);

	int getTeamID() const;
	bool isDead() const;

	void takeDamage(float damage);
	float getHealth() const;
	float getShield() const;

	Unit* getUnit();

private:
	void updateShield();
	void updateAttack();
	void updateTrailEffect();

	void move();
	void rotate();

private:
	//�ړI�n
	Vec3 m_Destination;

	//���g�̕���
	Vec3 m_Direction;

	//�e�픻��p�R���C�_�[
	CircleColliderB2* m_pCollider;
	CircleColliderB2* m_pTrigger;

	//�����̏������Ă��郆�j�b�g
	Unit* m_pUnit;

	//�U���Ώۂ̌��
	std::unordered_set<UnitObject*> m_TargetCandidates;

	ValueMap* m_pValueMap;

	float m_Health;
	float m_Shield;

	bool m_IsDead;

	//�V�[���h�񕜊J�n�܂ł̃^�C�}�[
	Timer m_ShieldRegenTimer;

	//�˒��������ł������U��
	Attack* m_pLongestMainAttack;

	//�U���N���X(���C��)
	std::vector<Attack*> m_MainAttacks;

	//�U���N���X(�T�u)
	std::vector<Attack*> m_SubAttacks;

	//�G�t�F�N�g
	CubeTrailEffect* m_pCubeTrailEffect;
};