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
	//���}�b�v
	ValueMap m_ValueMap1;
	ValueMap m_ValueMap2;

	//�}�e���A��
	InstancingMaterial* m_pInstancingMaterial;
	BBModelMaterial* m_pBBModelMaterial;
	ValueMapMaterial* m_pValueMapMaterial;

	//Blockbench���f���p�L���[�u
	DX12Mesh* m_pBBCube;

	//Unit�`��⏕
	std::unordered_map<std::string, UnitRenderHelper*> m_UnitRenderHelpers;
	//�G�t�F�N�g�`��⏕
	EffectRenderHelper* m_pEffectRenderHelper;

	//���j�b�g�I��
	UnitSelector* m_pUnitSelector;

	//�v���C���[
	Player* m_pPlayer;
	AIPlayer* m_pAIPlayer;

	//�O�t���[���̃J�����ʒu
	Vec3 m_PreCameraPos;
	//�O�t���[���̃J������]
	Vec3 m_PreCameraAngles;

	//UI�n
	UIUnitList* m_pUIUnitList;
	UIUnitPlacer* m_pUIUnitPlacer;

	//�w�i�X�v���C�g��Transform
	Transform* m_pBGSpriteTr;
	//���s�\���e�L�X�g
	D2DTextRenderer* m_pWinLoseText;

	//�X�e�[�W��
	std::string m_StageName;

	//�V�[���J�ڌn
	std::vector<GUISpriteRenderer*> m_SpriteRenderers;
	std::vector<Action::ActionManager*> m_ActionManagers;
	bool m_IsSceneChangeBegin;
	Timer m_SceneChangeTimer;
};