#include "UIUnitList.h"

#include "Component\Utility\Action\Actions.h"
#include "Component\Utility\Action\ActionManager.h"

#include "Player\IPlayer.h"
#include "Unit\Unit.h"
#include "Unit\UnitContainer.h"
#include "Unit\UnitSelector.h"

#include "UI\UIButton.h"
#include "UI\UICombatStatus.h"
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

	IGameMediator* pGameMediator = getUser().getGameMediator();

	Camera* pCamera = pGameMediator->getMainCamera();
	Transform* pCameraTr = &pCamera->getTransform();
	Action::ActionManager* pActionManager = pCamera->getUser().addComponent<Action::ActionManager>();

	int i = 0;
	for (auto pUnit : units)
	{
		auto pButtonObject = new GameObject(pGameMediator);
		pButtonObject->getTransform().setLocalPosition(Vec3((unitCardWidth + spacePerUnitCard) * i - positionOffset, -300.0f, 1.0f));
		pButtonObject->getTransform().setLocalScale(Vec3(unitCardWidth, unitCardHeight, 1.0f));

		auto pButton = pButtonObject->addComponent<UIButton>();
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

				Vec3 diff = pCameraTr->getLocalPosition() - destination;
				destination = destination + diff.normalized() * pUnit->getWidth() * pUnit->getSpacePerObject() * 0.75f;

				//�O��Action���������璆�f
				if (pActionManager->actionCount() != 0)
					pActionManager->forceNext();

				pActionManager->enqueueAction(new Action::EaseOutQuart(new Action::MoveTo(destination, 1.0f)));
			}
		);


		//�X�e�[�^�X�\���p�e�I�u�W�F�N�g
		auto pStatsDisplayObject = new GameObject(pGameMediator);
		pStatsDisplayObject->setParent(&getUser());
		pStatsDisplayObject->getTransform().setLocalPosition(Vec3((unitCardWidth + spacePerUnitCard) * i - positionOffset, -300.0f, 1.0f));
		pStatsDisplayObject->getTransform().setLocalScale(Vec3(1.0f));

		//���j�b�g�̉摜(���͐F����)
		auto pUnitCardObject = new GameObject(pGameMediator);
		pUnitCardObject->setParent(pStatsDisplayObject);
		pUnitCardObject->getTransform().setLocalPosition(Vec3(0.0f, 0.0f));
		pUnitCardObject->getTransform().setLocalScale(Vec3(unitCardWidth, unitCardHeight, 1.0f));
		auto pUnitCard = pUnitCardObject->addComponent<UIUnitCard>();
		pUnitCard->init(pUnit->getUnitStats());

		auto pHealthObject = new GameObject(pGameMediator);
		pHealthObject->setParent(pStatsDisplayObject);
		pHealthObject->getTransform().setLocalPosition(Vec3(0.0f, unitCardHeight * 0.5f + sliderHeight * 0.5f, 1.0f));
		pHealthObject->getTransform().setLocalScale(Vec3(unitCardWidth, sliderHeight, 1.0f));

		//���j�b�g�̗͕̑\��
		auto pHealthSlider = pHealthObject->addComponent<UISlider>();
		pHealthSlider->setWidth(1.0f);
		pHealthSlider->setDirection(UISlider::Direction::RIGHT);
		pHealthSlider->setColor(Color(DirectX::Colors::Green));
		pHealthSlider->setCurrentValue(pUnit->getHealth());
		pHealthSlider->setMaxValue(pUnit->getHealth());
		pHealthSlider->setTextureByName("BoxFill");

		//�X���C�_�[�⏕(HP)
		auto pHealthHelper = pHealthObject->addComponent<UISliderHelper<Unit>>();
		pHealthHelper->init(pHealthSlider, pUnit, [](Unit* pUnit)
			{
				return pUnit->getHealth();
			}
		);

		//�퓬���A�C�R���̕\��
		auto pCombatUIObject = new GameObject(pGameMediator);
		pCombatUIObject->setParent(pStatsDisplayObject);
		pCombatUIObject->getTransform().setLocalPosition(Vec3(0.0f, unitCardHeight * 0.8f, 0.0f));
		pCombatUIObject->getTransform().setLocalScale(Vec3(16.0f, 16.0f, 1.0f));
		auto pCombatStatus = pCombatUIObject->addComponent<UICombatStatus>();
		pCombatStatus->init(pUnit);

		//�V�[���h������Ȃ�
		if (pUnit->getShield() > 0.0f)
		{
			auto pShieldObj = new GameObject(pGameMediator);
			pShieldObj->setParent(pStatsDisplayObject);
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
