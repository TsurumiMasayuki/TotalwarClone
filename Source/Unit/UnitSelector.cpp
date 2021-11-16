#include "UnitSelector.h"
#include "Actor\Base\GameObject.h"
#include "Component\Box2D\CircleColliderB2.h"
#include "Component\Cursor.h"
#include "Device\GameDevice.h"
#include "Unit\Unit.h"
#include "Unit\UnitObject.h"
#include "Unit\UnitStats.h"
#include "Utility\ModelGameObjectHelper.h"
#include "Math\MathUtility.h"

#include "GameState.h"
#include "Graphics\Material\ValueMapMaterial.h"

void UnitSelector::onStart()
{
	m_InputInterval.setUseUnscaledTime(true);
	m_InputInterval.setMaxTime(0.25f);
}

void UnitSelector::onUpdate()
{
	const Vec3& cursorPoint = m_pCursor->getCursorPoint();
	getTransform().setLocalPosition(cursorPoint);

	//入力インターバル更新
	m_InputInterval.update();
	//入力インターバル中ならreturn
	if (!m_InputInterval.isTime()) return;

	const Input& input = GameDevice::getInput();

	//ユニット選択の更新
	updateUnitSelecting();

	//何も選択されていないならreturn
	if (m_pSelectedUnit == nullptr) return;

	//右クリックしたら攻撃
	if (input.isMouseButtonDown(0) &&
		m_pAttackTargetUnit != nullptr)
	{
		m_pSelectedUnit->setTarget(m_pAttackTargetUnit);
	}

	//ユニット配置の更新
	updateUnitPlacement();

	//配置予測表示中でないなら選択中のユニットを強調表示
	if (!input.isMouseButton(1))
	{
		setInstanceInfo(m_pSelectedUnit->getTransform().getLocalPosition(),
			m_pSelectedUnit->getAngle(),
			m_pSelectedUnit->getWidth()
		);
	}

	//デリートキーが押されたら
	if (input.isKeyDown(DIK_DELETE))
	{
		//選択したユニットをオブジェクトごと破棄
		m_pSelectedUnit->getUser().setActive(false);
		m_pSelectedUnit = nullptr;
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
	pCollider->setBodyType(b2_dynamicBody);
	pCollider->setGroupIndex(0);

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

	//前のプレビュー用オブジェクトをクリア
	for (auto pObj : m_PreviewObjects)
	{
		m_ObjPlacement.removeObject(&pObj->getTransform());
		pObj->destroy();
	}
	m_PreviewObjects.clear();

	//配置補助にオブジェクトを登録
	for (int i = 0; i < m_pSelectedUnit->getUnitStats()->m_ObjectCount; i++)
	{
		auto pObj = new GameObject(getUser().getGameMediator());
		m_PreviewObjects.emplace_back(pObj);
		m_ObjPlacement.addObject(&pObj->getTransform());
	}
}

void UnitSelector::updateUnitSelecting()
{
	const Input& input = GameDevice::getInput();

	//左クリックしたら選択
	if (input.isMouseButtonDown(0) &&
		m_pTargetUnit != nullptr)
	{
		selectUnit(m_pTargetUnit);
	}
}

void UnitSelector::updateUnitPlacement()
{
	//3D空間上でカーソルが指している場所を取得
	const Vec3& cursorPoint = m_pCursor->getCursorPoint();

	const Input& input = GameDevice::getInput();

	if (input.isMouseButtonDown(1))
	{
		//ユニット配置の記録開始
		m_UnitPlacePosBegin = cursorPoint;
	}

	//配置予測を表示
	if (input.isMouseButton(1))
	{
		Vec3 unitPlacePosEnd = cursorPoint;
		Vec3 diff = unitPlacePosEnd - m_UnitPlacePosBegin;
		diff.y = 0.0f;
		Vec3 placePos = m_UnitPlacePosBegin + diff.normalized() * diff.length() * 0.5f;
		float angle = MathUtility::toDegree(-std::atan2f(diff.z, diff.x));
		int width = diff.length() / m_pSelectedUnit->getSpacePerObject();

		setInstanceInfo(placePos, angle, width);
	}

	//攻撃中でないなら移動処理終了
	if (input.isMouseButtonUp(1) &&
		m_pAttackTargetUnit == nullptr)
	{
		//ユニット配置の記録終了
		Vec3 unitPlacePosEnd = cursorPoint;
		Vec3 diff = unitPlacePosEnd - m_UnitPlacePosBegin;
		Vec3 placePos = m_UnitPlacePosBegin + diff.normalized() * diff.length() * 0.5f;

		if (Game::g_GameState == Game::GameState::PreparePhase)
		{
			//準備フェーズならワープ
			m_pSelectedUnit->setPosition(placePos,
				-MathUtility::toDegree(std::atan2f(diff.z, diff.x)),
				diff.length() / m_pSelectedUnit->getSpacePerObject()
			);
		}
		else
		{
			//目的地設定
			m_pSelectedUnit->setDestination(placePos,
				-MathUtility::toDegree(std::atan2f(diff.z, diff.x)),
				diff.length() / m_pSelectedUnit->getSpacePerObject(),
				true
			);
		}

		//InstancedRendererに空のデータを渡して非表示にする
		std::vector<PreviewObjInstance> instances;
		m_pPreviewObjRenderer->setInstanceInfo(instances);
	}
}

void UnitSelector::setInstanceInfo(const Vec3& placePos, float angle, int width)
{
	//プレビューの設定
	m_ObjPlacement.setSpacePerObject(m_pSelectedUnit->getSpacePerObject());
	m_ObjPlacement.setBasePos(placePos);
	m_ObjPlacement.setAngle(angle);
	m_ObjPlacement.setWidth(width);

	//プレビューの座標計算
	std::vector<Vec3> previewObjPositions;
	m_ObjPlacement.calculateObjectPositions(previewObjPositions);

	const float modelScaling = 0.05f;
	const Vec3& scale = m_pSelectedUnit->getTransform().getLocalScale();
	auto scaling = DirectX::XMMatrixScaling(scale.x * modelScaling, 0.1f * modelScaling, scale.z * modelScaling);
	auto offsetMat = DirectX::XMMatrixTranslation(0.0f, -10.0f, 0.0f);

	//InstancedRenderer用データ作成
	std::vector<PreviewObjInstance> instances;
	for (auto& position : previewObjPositions)
	{
		instances.emplace_back();
		auto translate = DirectX::XMMatrixTranslationFromVector(position.toXMVector());
		auto world = DirectX::XMMatrixTranspose(scaling * translate * offsetMat);
		DirectX::XMStoreFloat4x4(&instances.back().instanceMat, world);
		DirectX::XMStoreFloat4(&instances.back().instanceColor, DirectX::Colors::Orange);
	}

	//InstancedRendererにデータを渡す
	m_pPreviewObjRenderer->setInstanceInfo(instances);
}
