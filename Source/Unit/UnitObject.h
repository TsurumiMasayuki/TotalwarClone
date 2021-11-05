#pragma once
#include <vector>
#include "Component\Base\AbstractComponent.h"
#include "Utility\Timer.h"

class Unit;
class Attack;
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
		Charge,	//�ˌ�(�U������ʒu�܂ňړ�)
		Attack, //�퓬
		Dead,	//���S
	};

public:
	virtual void onStart() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;

	void init(Unit* pUnit, ValueMap* pValueMap);

	//���W��ύX
	void setPosition(const Vec3& position);
	//�ړI�n��ݒ�
	void setDestination(const Vec3& destination, bool isMoveCommand = true);
	void move();
	void rotate();

	void onCollisionEnter(UnitObject* pUnitObject);
	void onCollisionStay(UnitObject* pUnitObject);
	void onCollisionExit(UnitObject* pUnitObject);

	void onTriggerEnter(UnitObject* pUnitObject);
	void onTriggerStay(UnitObject* pUnitObject);
	void onTriggerExit(UnitObject* pUnitObject);

	const State& getState() const;

	int getTeamID() const;

	void takeDamage(float damage);
	float getHealth() const;
	float getShield() const;

	Unit* getUnit();

private:
	void stateTransition();
	void trySetTargetObject(UnitObject* pTargetObject, const State& nextState);

	void setState(const State& newState);

	void updateShield();

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

	//�^�[�Q�b�g�I�u�W�F�N�g
	UnitObject* m_pTargetObject;

	ValueMap* m_pValueMap;

	float m_Health;
	float m_Shield;

	State m_State;

	//�V�[���h�񕜊J�n�܂ł̃^�C�}�[
	Timer m_ShieldRegenTimer;

	//�U���N���X(���C��)
	std::vector<Attack*> m_MainAttacks;

	//�U���N���X(�T�u)
	std::vector<Attack*> m_SubAttacks;
};