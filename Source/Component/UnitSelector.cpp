#include "UnitSelector.h"
#include "Actor\Base\GameObject.h"
#include "Component\Box2D\CircleColliderB2.h"
#include "Component\Cursor.h"
#include "Device\GameDevice.h"
#include "Unit\Unit.h"
#include "Unit\UnitObject.h"
#include "Utility\ModelGameObjectHelper.h"
#include "Math\MathUtility.h"

#include "Graphics\Material\ValueMapMaterial.h"

void UnitSelector::onStart()
{
	m_InputInterval.setUseUnscaledTime(true);
	m_InputInterval.setMaxTime(0.25f);
}

void UnitSelector::onUpdate()
{
	//3D空間上でカーソルが指している場所を取得
	const Vec3& cursorPoint = m_pCursor->getCursorPoint();
	getTransform().setLocalPosition(cursorPoint);

	//入力インターバル更新
	m_InputInterval.update();
	//入力インターバル中ならreturn
	if (!m_InputInterval.isTime()) return;

	//中クリックしたら選択(ターゲットがいないなら選択解除)
	if (GameDevice::getInput().isMouseButtonDown(2) &&
		m_pTargetUnit != nullptr)
	{
		selectUnit(m_pTargetUnit);
	}

	if (m_pSelectedUnit == nullptr) return;

	//左クリックしたら移動または攻撃
	if (GameDevice::getInput().isMouseButtonDown(0))
	{
		if (m_pAttackTargetUnit != nullptr)
		{
			m_pSelectedUnit->setTarget(m_pAttackTargetUnit);
		}

		//ユニット配置の記録開始
		m_UnitPlacePosBegin = cursorPoint;
	}

	//配置予測を表示
	if (GameDevice::getInput().isMouseButton(0))
	{

		Vec3 unitPlacePosEnd = cursorPoint;
		Vec3 diff = unitPlacePosEnd - m_UnitPlacePosBegin;
		diff.y = 0.0f;
		Vec3 placePos = m_UnitPlacePosBegin + diff.normalized() * diff.length() * 0.5f;

		std::vector<Vec3> previewObjPositions;
		m_pSelectedUnit->calculateObjectPositions(previewObjPositions,
			placePos,
			-std::atan2f(diff.z, diff.x),
			diff.length() / m_pSelectedUnit->getSpacePerObject()
		);

		const float modelScaling = 0.05f;
		const Vec3& scale = m_pSelectedUnit->getTransform().getLocalScale();

		//InstancedRenderer用データ作成
		std::vector<PreviewObjInstance> instances;
		for (auto& position : previewObjPositions)
		{
			instances.emplace_back();
			auto translate = DirectX::XMMatrixTranslationFromVector(position.toXMVector());
			auto scaling = DirectX::XMMatrixScaling(scale.x * modelScaling, 0.1f * modelScaling, scale.z * modelScaling);
			translate = DirectX::XMMatrixTranspose(scaling * translate);
			DirectX::XMStoreFloat4x4(&instances.back().instanceMat, translate);
			DirectX::XMStoreFloat4(&instances.back().instanceColor, DirectX::Colors::Orange);
		}

		//InstancedRendererにデータを渡す
		m_pPreviewObjRenderer->setInstanceInfo(instances);
	}

	//攻撃中でないなら移動処理終了
	if (GameDevice::getInput().isMouseButtonUp(0) &&
		m_pAttackTargetUnit == nullptr)
	{
		//ユニット配置の記録終了
		Vec3 unitPlacePosEnd = cursorPoint;
		Vec3 diff = unitPlacePosEnd - m_UnitPlacePosBegin;
		diff.y = 0.0f;
		Vec3 placePos = m_UnitPlacePosBegin + diff.normalized() * diff.length() * 0.5f;
		m_pSelectedUnit->setDestination(placePos,
			-MathUtility::toDegree(std::atan2f(diff.z, diff.x)),
			diff.length() / m_pSelectedUnit->getSpacePerObject()
		);

		//InstancedRendererに空のデータを渡して非表示にする
		std::vector<PreviewObjInstance> instances;
		m_pPreviewObjRenderer->setInstanceInfo(instances);
	}
}

void UnitSelector::onTriggerStay(GameObject* pHit)
{
	auto pUnitObject = pHit->getComponent<UnitObject>();
	if (pUnitObject == nullptr) return;
	if (pUnitObject->getTeamID() != m_TeamID)
	{
		//違うチームなら攻撃対象候補にする
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

	//ターゲット解除
	if (pUnitObject->getUnit() == m_pTargetUnit)
	{
		m_pTargetUnit = nullptr;
	}

	//攻撃対象解除
	if (pUnitObject->getUnit() == m_pAttackTargetUnit)
	{
		m_pAttackTargetUnit = nullptr;
	}
}

void UnitSelector::init(Cursor* pCursor, int teamID, ValueMapMaterial* pMaterial)
{
	m_pCursor = pCursor;
	m_TeamID = teamID;

	//ユニット検出用コライダー追加
	auto pCollider = getUser().addComponent<CircleColliderB2>();
	pCollider->setRadius(1.0f);
	pCollider->setTrigger(true);
	pCollider->setBodyType(b2_staticBody);

	auto pRendererObj = ModelGameObjectHelper::instantiateModel<PreviewObjInstance>(getUser().getGameMediator(),
		GameDevice::getModelManager().getModel("Cube"),
		true);
	m_pPreviewObjRenderer = pRendererObj->getChildren().at(0)->getComponent<InstancedRenderer<PreviewObjInstance>>();
	m_pPreviewObjRenderer->setMaterial(pMaterial);
}

void UnitSelector::selectUnit(Unit* pUnit)
{
	m_pSelectedUnit = pUnit;
	m_InputInterval.reset();
}

void UnitSelector::instantiatePreviewObjects()
{

}
