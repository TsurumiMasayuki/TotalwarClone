#pragma once
#include <vector>
#include <unordered_map>
#include "AI\ValueMap.h"
#include "Scene\Base\AbstractScene.h"
#include "Effect\EffectRenderHelper.h"
#include "Utility\Timer.h"

class GameObject;
class DX12Mesh;

class Unit;
class UnitRenderHelper;
class UnitSelector;

class UIUnitPlacer;
class UIUnitList;

class InstancingMaterial;
class BBModelMaterial;
class ValueMapMaterial;

class Model;
class Player;
class AIPlayer;

class Transform;
class GUISpriteRenderer;
class D2DTextRenderer;

class GameScene
	: public AbstractScene
{
public:
	virtual std::string nextScene() override;
	virtual bool isEnd() override;
	virtual void start() override;
	virtual void update() override;
	virtual void shutdown() override;

	virtual void lateUpdate() override;

	void setStage(const std::string& stageName);

private:
	//情報マップ
	ValueMap m_ValueMap1;
	ValueMap m_ValueMap2;

	//マテリアル
	InstancingMaterial* m_pInstancingMaterial;
	BBModelMaterial* m_pBBModelMaterial;
	ValueMapMaterial* m_pValueMapMaterial;

	//Blockbenchモデル用キューブ
	DX12Mesh* m_pBBCube;

	//Unit描画補助
	std::unordered_map<std::string, UnitRenderHelper*> m_UnitRenderHelpers;
	//エフェクト描画補助
	EffectRenderHelper* m_pEffectRenderHelper;

	//ユニット選択
	UnitSelector* m_pUnitSelector;

	//プレイヤー
	Player* m_pPlayer;
	AIPlayer* m_pAIPlayer;

	//前フレームのカメラ位置
	Vec3 m_PreCameraPos;
	//前フレームのカメラ回転
	Vec3 m_PreCameraAngles;

	//UI系
	UIUnitList* m_pUIUnitList;
	UIUnitPlacer* m_pUIUnitPlacer;

	//背景スプライトのTransform
	Transform* m_pBGSpriteTr;
	//勝敗表示テキスト
	D2DTextRenderer* m_pWinLoseText;

	//ステージ名
	std::string m_StageName;

	//シーン遷移系
	std::vector<GUISpriteRenderer*> m_SpriteRenderers;
	std::vector<Action::ActionManager*> m_ActionManagers;
	bool m_IsSceneChangeBegin;
	Timer m_SceneChangeTimer;
};