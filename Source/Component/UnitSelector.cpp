#include "UnitSelector.h"
#include "Actor\Base\GameObject.h"
#include "Component\Box2D\CircleColliderB2.h"
#include "Component\Cursor.h"
#include "Device\GameDevice.h"
#include "Unit\Unit.h"
#include "Unit\UnitObject.h"
#include "Math\MathUtility.h"

void UnitSelector::onStart()
{
	m_InputInterval.setUseUnscaledTime(true);
	m_InputInterval.setMaxTime(0.25f);
}

void UnitSelector::onUpdate()
{
	//3D��ԏ�ŃJ�[�\�����w���Ă���ꏊ���擾
	const Vec3& cursorPoint = m_pCursor->getCursorPoint();
	getTransform().setLocalPosition(cursorPoint);

	//���̓C���^�[�o���X�V
	m_InputInterval.update();
	//���̓C���^�[�o�����Ȃ�return
	if (!m_InputInterval.isTime()) return;

	//���N���b�N������I��(�^�[�Q�b�g�����Ȃ��Ȃ�I������)
	if (GameDevice::getInput().isMouseButtonDown(2) &&
		m_pTargetUnit != nullptr)
	{
		selectUnit(m_pTargetUnit);
	}

	//���N���b�N������ړ��܂��͍U��
	if (GameDevice::getInput().isMouseButtonDown(0) &&
		m_pSelectedUnit != nullptr)
	{
		if (m_pAttackTargetUnit != nullptr)
		{
			m_pSelectedUnit->setTarget(m_pAttackTargetUnit);
		}

		//���j�b�g�z�u�̋L�^�J�n
		m_UnitPlacePosBegin = cursorPoint;
	}

	//�U�����łȂ��Ȃ�ړ������I��
	if (GameDevice::getInput().isMouseButtonUp(0) &&
		m_pSelectedUnit != nullptr &&
		m_pAttackTargetUnit == nullptr)
	{
		//���j�b�g�z�u�̋L�^�I��
		Vec3 unitPlacePosEnd = cursorPoint;
		Vec3 diff = unitPlacePosEnd - m_UnitPlacePosBegin;
		Vec3 placePos = m_UnitPlacePosBegin + diff.normalized() * diff.length() * 0.5f;
		m_pSelectedUnit->setDestination(placePos,
			MathUtility::toDegree(std::atan2f(diff.y, diff.x)),
			diff.length() / m_pSelectedUnit->getSpacePerObject()
		);
	}
}

void UnitSelector::onTriggerStay(GameObject* pHit)
{
	auto pUnitObject = pHit->getComponent<UnitObject>();
	if (pUnitObject == nullptr) return;
	if (pUnitObject->getTeamID() != m_TeamID)
	{
		//�Ⴄ�`�[���Ȃ�U���Ώی��ɂ���
		m_pAttackTargetUnit = pUnitObject->getUnit();
		return;
	}
	m_pTargetUnit = pUnitObject->getUnit();
}

void UnitSelector::onTriggerExit(GameObject* pHit)
{
	auto pUnitObject = pHit->getComponent<UnitObject>();
	if (pUnitObject == nullptr) return;
	if (pUnitObject->getTeamID() == m_TeamID) return;

	//�^�[�Q�b�g����
	if (pUnitObject->getUnit() == m_pTargetUnit)
	{
		m_pTargetUnit = nullptr;
	}

	//�U���Ώۉ���
	if (pUnitObject->getUnit() == m_pAttackTargetUnit)
	{
		m_pAttackTargetUnit = nullptr;
	}
}

void UnitSelector::init(Cursor* pCursor, int teamID)
{
	m_pCursor = pCursor;
	m_TeamID = teamID;

	//���j�b�g���o�p�R���C�_�[�ǉ�
	auto pCollider = getUser().addComponent<CircleColliderB2>();
	pCollider->setRadius(1.0f);
	pCollider->setTrigger(true);
	pCollider->setBodyType(b2_staticBody);
}

void UnitSelector::selectUnit(Unit* pUnit)
{
	m_pSelectedUnit = pUnit;
	m_InputInterval.reset();
}
