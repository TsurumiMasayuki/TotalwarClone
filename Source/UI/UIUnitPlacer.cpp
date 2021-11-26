#include "UIUnitPlacer.h"

#include <filesystem>

#include "Actor\Base\GameObject.h"
#include "Component\Cursor.h"
#include "Component\Graphics\D2DTextRenderer.h"
#include "Device\GameDevice.h"
#include "Graphics\DX12\Material\DefaultMaterials.h"
#include "Utility\StringUtility.h"

#include "Player\Player.h"

#include "UI\UIButton.h"
#include "UI\UIUnitCard.h"
#include "Utility\JsonFileManager.h"
#include "Utility\ModelGameObjectHelper.h"

#include "Unit\Unit.h"
#include "Unit\UnitSelector.h"

#include "GameState.h"

void UIUnitPlacer::onStart()
{
}

void UIUnitPlacer::onUpdate()
{
	if (Game::g_GameState == Game::GameState::CombatPhaseBegin)
	{
		getUser().setActive(false);
	}
}

void UIUnitPlacer::init(Cursor* pCursor,
	Player* pPlayer,
	UnitSelector* pUnitSelector,
	ValueMap* pValueMap,
	const std::unordered_map<std::string, UnitRenderHelper*>* pRenderHelpers,
	EffectRenderHelper* pEffectRenderHelper)
{
	m_pCursor = pCursor;
	m_pRenderHelpers = pRenderHelpers;

	const float unitCardWidth = 60.0f;
	const float unitCardHeight = 60.0f;
	const float sliderHeight = 8.0f;
	const float spacePerUnitCard = 8.0f;
	const float unitCardY = 200.0f;

	std::vector<UnitStats> unitStatsList;
	auto& unitStatsManager = JsonFileManager<UnitStats>::getInstance();

	//Resources/UnitStats�̒��𑖍�
	for (const auto& directory : std::filesystem::directory_iterator("Resources/UnitStats"))
	{
		const auto& path = directory.path();
		auto fileName = path.filename();
		//�g���q�����ɕϊ�
		std::string noExt = fileName.string().substr(0, fileName.string().size() - (size_t)5);
		unitStatsList.emplace_back(unitStatsManager.get(noExt));
	}

	const float positionOffset = (unitCardWidth + spacePerUnitCard) * (float)(unitStatsList.size() - 1) * 0.5f;

	IGameMediator* pGameMediator = getUser().getGameMediator();
	auto pModel = GameDevice::getModelManager().getModel("Sphere");
	for (int i = 0; i < (int)unitStatsList.size(); i++)
	{
		const auto& unitStats = unitStatsList.at(i);

		auto pButtonObj = new GameObject(getUser().getGameMediator());
		pButtonObj->getTransform().setLocalPosition(Vec3((unitCardWidth + spacePerUnitCard) * i - positionOffset, unitCardY, 1.0f));
		pButtonObj->getTransform().setLocalScale(Vec3(unitCardWidth, unitCardHeight, 1.0f));
		pButtonObj->setParent(&getUser());

		auto pButton = pButtonObj->addComponent<UIButton>();
		//���j�b�g�z�u�֐���ݒ�
		pButton->setOnMouseButtonDown(UIButton::MouseButtons::Left,
			[pGameMediator, pUnitSelector, pPlayer, unitStats, pValueMap, pRenderHelpers, pEffectRenderHelper]()
			{
				//�z�u�p�G�l���M�[������������return
				if (pPlayer->getEnergy() - unitStats.m_EnergyCost < 0)
					return;

				//���j�b�g�p�I�u�W�F�N�g����
				auto pUnitObj = new GameObject(pGameMediator);
				//���j�b�g����
				auto pUnit = pUnitObj->addComponent<Unit>();
				pUnit->init(pPlayer, &unitStats, pValueMap, pRenderHelpers->at(unitStats.m_Name), pEffectRenderHelper);
				//���W�ݒ�(TODO:�v���C���[�̔z�u�͈͂̐^�񒆂ɂȂ�悤�ɕύX����)
				pUnit->setPosition(Vec3(0.0f, 0.0f, 0.0f), 0.0f, 10);
				//���j�b�g�o�^
				pPlayer->addUnit(pUnit);
				//�Z���N�^�[�őI��
				pUnitSelector->selectUnit(pUnit);
			}
		);

		//���j�b�g�̉摜(���͐F����)
		auto pUnitCardObj = new GameObject(getUser().getGameMediator());
		pUnitCardObj->setParent(&getUser());
		pUnitCardObj->getTransform().setLocalPosition(Vec3((unitCardWidth + spacePerUnitCard) * i - positionOffset, unitCardY, 1.0f));
		pUnitCardObj->getTransform().setLocalScale(Vec3(unitCardWidth, unitCardHeight, 1.0f));
		auto pUnitCard = pUnitCardObj->addComponent<UIUnitCard>();
		pUnitCard->init(&unitStats);

		//���j�b�g��
		auto pUnitNameObj = new GameObject(getUser().getGameMediator());
		pUnitNameObj->setParent(&getUser());
		pUnitNameObj->getTransform().setLocalPosition(Vec3((unitCardWidth + spacePerUnitCard) * i - positionOffset, unitCardY - unitCardHeight * 0.75f, 1.0f));
		pUnitNameObj->getTransform().setLocalScale(Vec3(1.0f, 1.0f, 1.0f));
		D2DTextRenderer* pNameRenderer = pUnitNameObj->addComponent<D2DTextRenderer>();
		pNameRenderer->setFont(L"Meiryo UI",
			L"ja-jp",
			8.0f
		);
		pNameRenderer->setFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_MEDIUM);
		pNameRenderer->setFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
		pNameRenderer->setFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
		pNameRenderer->setColor(Color(0.0f, 0.0f, 0.0f, 1.0f));
		pNameRenderer->setTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
		pNameRenderer->setParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		pNameRenderer->setTextRect(0.0f, 0.0f, 1280.0f, 720.0f);
		pNameRenderer->setText(StringUtility::ToWString(unitStats.m_DisplayName));

		//���j�b�g�R�X�g
		auto pUnitCostObj = new GameObject(getUser().getGameMediator());
		pUnitCostObj->setParent(&getUser());
		pUnitCostObj->getTransform().setLocalPosition(Vec3((unitCardWidth + spacePerUnitCard) * i - positionOffset, unitCardY - unitCardHeight, 1.0f));
		pUnitCostObj->getTransform().setLocalScale(Vec3(1.0f, 1.0f, 1.0f));
		D2DTextRenderer* pCostRenderer = pUnitCostObj->addComponent<D2DTextRenderer>();
		pCostRenderer->setFont(L"Meiryo UI",
			L"ja-jp",
			10.0f
		);
		pCostRenderer->setFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_MEDIUM);
		pCostRenderer->setFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
		pCostRenderer->setFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
		pCostRenderer->setColor(Color(0.0f, 0.0f, 0.0f, 1.0f));
		pCostRenderer->setTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
		pCostRenderer->setParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		pCostRenderer->setTextRect(0.0f, 0.0f, 1280.0f, 720.0f);
		pCostRenderer->setText(std::to_wstring(unitStats.m_EnergyCost));
	}
}
