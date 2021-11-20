#include "UnitSelector.h"
#include "Actor\Base\GameObject.h"
#include "Component\Box2D\CircleColliderB2.h"
#include "Component\Graphics\SpriteRenderer.h"
#include "Component\Utility\Action\Actions.h"
#include "Component\Utility\Action\ActionManager.h"
#include "Device\GameDevice.h"
#include "Utility\ModelGameObjectHelper.h"
#include "Math\MathUtility.h"

#include "Component\Cursor.h"

#include "GameState.h"
#include "Graphics\Material\ValueMapMaterial.h"

#include "Unit\Unit.h"
#include "Unit\UnitObject.h"
#include "Unit\UnitStats.h"

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

	//左クリックしたら攻撃
	if (input.isMouseButtonDown(0) &&
		m_pAttackTargetUnit != nullptr &&
		Game::g_GameState == Game::GameState::CombatPhase)
	{
		//プレイヤーの行動としてターゲット指定
		m_pSelectedUnit->setTarget(m_pAttackTargetUnit, true);

		//エフェクト設定
		const Vec3& selectPosition = m_pSelectedUnit->getTransform().getLocalPosition();
		const Vec3& targetPosition = m_pAttackTargetUnit->getTransform().getLocalPosition();

		Vec3 diff = targetPosition - selectPosition;
		float radian = std::atan2f(diff.x, diff.z);
		float angle = MathUtility::toDegree(radian);

		auto& transform = m_pAttackDisplayObj->getTransform();
		transform.setLocalPosition(selectPosition + diff.normalized() * diff.length() * 0.5f);
		transform.setLocalAngles(Vec3(90.0f, angle, 0.0f));
		transform.setLocalScale(Vec3(3.0f, diff.length(), 1.0f));

		m_pAttackDisplayObj->getComponent<Action::ActionManager>()->enqueueAction(new Action::ScaleTo(Vec3(0.0f, diff.length(), 0.0f), 1.0f));
	}

	//ユニット配置の更新
	updateUnitPlacement();

	//配置予測表示中でないなら選択中のユニットを強調表示
	if (!input.isMouseButton(1))
	{
		std::vector<PreviewObjInstance> instances;
		std::vector<UnitObject*>& objects = m_pSelectedUnit->getUnitObjects();

		//オブジェクトの少し下にハイライトを表示
		float modelScaling = 0.05f;
		auto scaling = DirectX::XMMatrixScaling(modelScaling, 0.0f, modelScaling);
		auto offsetMat = DirectX::XMMatrixTranslation(0.0f, -10.0f, 0.0f);
		for (auto object : objects)
		{
			if (object->isDead()) continue;

			instances.emplace_back();
			auto& instance = instances.back();
			DirectX::XMMATRIX world = scaling * object->getTransform().getTranslateMatrix() * offsetMat;
			DirectX::XMStoreFloat4x4(&instance.instanceMat, DirectX::XMMatrixTranspose(world));
			DirectX::XMStoreFloat4(&instance.instanceColor, DirectX::Colors::Orange);
		}

		m_pPreviewObjRenderer->setInstanceInfo(instances);
	}

	//準備フェーズ中にデリートキーが押されたら
	if (input.isKeyDown(DIK_DELETE) &&
		Game::g_GameState == Game::GameState::PreparePhase)
	{
		//選択したユニットを破棄(技術的な問題につき今は無効化)
		m_pSelectedUnit->getUser().setActive(false);
		m_pSelectedUnit = nullptr;
	}
}

void UnitSelector::onTriggerEnter(GameObject* pHit)
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
	pCollider->setGroupIndex(-1);

	auto pRendererObj = ModelGameObjectHelper::instantiateModel<PreviewObjInstance>(getUser().getGameMediator(),
		GameDevice::getModelManager().getModel("Cube"),
		true);
	m_pPreviewObjRenderer = pRendererObj->getChildren().at(0)->getComponent<InstancedRenderer<PreviewObjInstance>>();
	m_pPreviewObjRenderer->setMaterial(pMaterial);

	//攻撃指定時の矢印表示
	m_pAttackDisplayObj = new GameObject(getUser().getGameMediator());
	m_pAttackDisplayObj->getTransform().setLocalScale(Vec3::zero());
	auto pRenderer = m_pAttackDisplayObj->addComponent<SpriteRenderer>();
	pRenderer->setTexture(GameDevice::getTextureManager().getTexture("BoxFill"));
	pRenderer->setColor(Color(1.0f, 0.0f, 0.0f, 1.0f));
	m_pAttackDisplayObj->addComponent<Action::ActionManager>();
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

	if (m_pSelectedUnit == nullptr) return;

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
	//攻撃対象を選択しようとしているならreturn
	if (m_pAttackTargetUnit != nullptr) return;

	if (!m_InputInterval.isTime()) return;

	const Input& input = GameDevice::getInput();

	//左クリックしたら選択
	if (!input.isMouseButtonDown(0)) return;

	selectUnit(m_pTargetUnit);
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
	if (input.isMouseButtonUp(1))
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
	auto scaling = DirectX::XMMatrixScaling(modelScaling, 0.0f, modelScaling);
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
