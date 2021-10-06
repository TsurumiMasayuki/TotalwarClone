#pragma once
#include "Component\Base\AbstractComponent.h"
#include "Utility\Timer.h"

class Unit;
class CircleColliderB2;
class ValueMap;

class UnitObject
	: public AbstractComponent
{
public:
	enum class State
	{
		StandBy,//�ҋ@(�ʏ�)
		Move,	//�ʏ�ړ�
		Charge,	//�ˌ�
		Combat, //�퓬
		CombatStandBy,	//�ҋ@(�퓬)
		Blown,	//������΂��ꂽ
		Dead,	//���S
	};

public:
	virtual void onStart() override;
	virtual void onUpdate() override;

	void init(Unit* pUnit, ValueMap* pValueMap);

	//�ړI�n��ݒ�
	void setDestination(const Vec3& destination, bool moveCommand = true);
	void move();

	void onCollisionEnter(UnitObject* pUnitObject);
	void onCollisionStay(UnitObject* pUnitObject);
	void onCollisionExit(UnitObject* pUnitObject);

	void onTriggerEnter(UnitObject* pUnitObject);
	void onTriggerStay(UnitObject* pUnitObject);
	void onTriggerExit(UnitObject* pUnitObject);

	const State& getState() const;

	int getTeamID() const;

	void takeDamage(float damage);

private:
	void stateTransition();
	void attack();
	void trySetTargetObject(UnitObject* pTargetObject, const State& nextState);
	void trySetPredecessor(UnitObject* pPredecessor);
	void tryTakeOverTarget();

	void setState(const State& newState);

private:
	//�ړI�n
	Vec3 m_Destination;

	//�e�픻��p�R���C�_�[
	CircleColliderB2* m_pCollider;
	CircleColliderB2* m_pTrigger;

	//�����̏������Ă��郆�j�b�g
	Unit* m_pUnit;

	//�^�[�Q�b�g�I�u�W�F�N�g
	UnitObject* m_pTargetObject;

	//�퓬�̑O�C(�O�C���퓬�s�\�ɂȂ����ꍇ�A�����������p��)
	UnitObject* m_pPredecessor;
	//�퓬�̌�C(���g���퓬�s�\�ɂȂ����ꍇ�A��C�������p��)
	UnitObject* m_pSuccessor;

	ValueMap* m_pValueMap;

	float m_Health;

	State m_State;

	Timer m_AttackTimer;
};