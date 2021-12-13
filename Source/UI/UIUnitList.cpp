#include "UIUnitList.h"

#include "Component\Utility\Action\Actions.h"
#include "Component\Utility\Action\ActionManager.h"

#include "Player\IPlayer.h"
#include "Unit\Unit.h"
#include "Unit\UnitContainer.h"
#include "Unit\UnitSelector.h"

#include "UI\UIButton.h"
#include "UI\UIUnitCard.h"
#include "UI\UISlider.h"
#include "UI\UISliderHelper.h"

void UIUnitList::onStart()
{
}

void UIUnitList::onUpdate()
{
}

void UIUnitList::init(IPlayer* pPlayer, UnitSelector* pSelector)
{
	const float unitCardWidth = 60.0f;
	const float unitCardHeight = 96.0f;
	const float sliderHeight = 8.0f;
	const float spacePerUnitCard = 8.0f;

	m_pPlayer = pPlayer;
	const auto& units = m_pPlayer->getUnitContainer()->getUnits();
	const float positionOffset = (unitCardWidth + spacePerUnitCard) * (float)(units.size() - 1) * 0.5f;

	Camera* pCamera = getUser().getGameMediator()->getMainCamera();
	Transform* pCameraTr = &pCamera->getTransform();
	Action::ActionManager* pActionManager = pCamera->getUser().addComponent<Action::ActionManager>();

	int i = 0;
	for (auto pUnit : units)
	{
		auto pButtonObj = new GameObject(getUser().getGameMediator());
		pButtonObj->getTransform().setLocalPosition(Vec3((unitCardWidth + spacePerUnitCard) * i - positionOffset, -300.0f, 1.0f));
		pButtonObj->getTransform().setLocalScale(Vec3(unitCardWidth, unitCardHeight, 1.0f));

		auto pButton = pButtonObj->addComponent<UIButton>();
		//���j�b�g�I���֐���ݒ�
		pButton->setOnMouseButtonDown(UIButton::MouseButtons::Left, [pSelector, pUnit]()
			{
				pSelector->selectUnit(pUnit);
			}
		);

		//�J���������j�b�g�Ƀt�H�[�J�X����
		pButton->setOnMouseButtonDoubleClick(UIButton::MouseButtons::Left, [pCameraTr, pUnit, pActionManager]()
			{
				Vec3 destination = pUnit->getTransform().getLocalPosition();
				destination.y = pCameraTr->getLocalPosition().y;

				//�O��Action���������璆�f
				if (pActionManager->actionCount() != 0)
					pActionManager->forceNext();

				pActionManager->enqueueAction(new Action::EaseOutQuart(new Action::MoveTo(destination, 1.0f)));
			}
		);


		//�X�e�[�^�X�\���p�e�I�u�W�F�N�g
		auto pStatsDisplayObj = new GameObject(getUser().getGameMediator());
		pStatsDisplayObj->setParent(&getUser());
		pStatsDisplayObj->getTransform().setLocalPosition(Vec3((unitCardWidth + spacePerUnitCard) * i - positionOffset, -300.0f, 1.0f));
		pStatsDisplayObj->getTransform().setLocalScale(Vec3(1.0f));

		//���j�b�g�̉摜(���͐F����)
		auto pUnitCardObj = new GameObject(getUser().getGameMediator());
		pUnitCardObj->setParent(pStatsDisplayObj);
		pUnitCardObj->getTransform().setLocalPosition(Vec3(0.0f, 0.0f));
		pUnitCardObj->getTransform().setLocalScale(Vec3(unitCardWidth, unitCardHeight, 1.0f));
		auto pUnitCard = pUnitCardObj->addComponent<UIUnitCard>();
		pUnitCard->init(pUnit->getUnitStats());

		auto pHealthObj = new GameObject(getUser().getGameMediator());
		pHealthObj->setParent(pStatsDisplayObj);
		pHealthObj->getTransform().setLocalPosition(Vec3(0.0f, unitCardHeight * 0.5f + sliderHeight * 0.5f, 1.0f));
		pHealthObj->getTransform().setLocalScale(Vec3(unitCardWidth, sliderHeight, 1.0f));

		//���j�b�g�̗͕̑\��
		auto pHealthSlider = pHealthObj->addComponent<UISlider>();
		pHealthSlider->setWidth(1.0f);
		pHealthSlider->setDirection(UISlider::Direction::RIGHT);
		pHealthSlider->setColor(Color(DirectX::Colors::Green));
		pHealthSlider->setCurrentValue(pUnit->getHealth());
		pHealthSlider->setMaxValue(pUnit->getHealth());
		pHealthSlider->setTextureByName("BoxFill");

		//�X���C�_�[�⏕(HP)
		auto pHealthHelper = pHealthObj->addComponent<UISliderHelper<Unit>>();
		pHealthHelper->init(pHealthSlider, pUnit, [](Unit* pUnit)
			{
				return pUnit->getHealth();
			}
		);

		//�V�[���h������Ȃ�
		if (pUnit->getShield() > 0.0f)
		{
			auto pShieldObj = new GameObject(getUser().getGameMediator());
			pShieldObj->setParent(pStatsDisplayObj);
			pShieldObj->getTransform().setLocalPosition(Vec3(0.0f, unitCardHeight * 0.5f + sliderHeight * 0.5f));
			pShieldObj->getTransform().setLocalScale(Vec3(unitCardWidth, sliderHeight, 1.0f));

			//���j�b�g�̃V�[���h�\��
			auto pShieldSlider = pShieldObj->addComponent<UISlider>();
			pShieldSlider->setWidth(1.0f);
			pShieldSlider->setDirection(UISlider::Direction::RIGHT);
			pShieldSlider->setColor(Color(DirectX::Colors::Cyan));
			pShieldSlider->setCurrentValue(pUnit->getShield());
			pShieldSlider->setMaxValue(pUnit->getShield());
			pShieldSlider->setTextureByName("BoxFill");

			//�X���C�_�[�⏕(�V�[���h)
			auto pShieldHelper = pShieldObj->addComponent<UISliderHelper<Unit>>();
			pShieldHelper->init(pShieldSlider, pUnit, [](Unit* pUnit)
				{
					return pUnit->getShield();
				}
			);
		}

		i++;
	}
}
