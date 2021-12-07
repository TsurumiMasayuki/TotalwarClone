#include "UIBattleSlider.h"

#include "Actor\Base\GameObject.h"
#include "Component\Utility\Action\Actions.h"
#include "Component\Utility\Action\ActionManager.h"
#include "Device\GameDevice.h"

#include "Player\Player.h"
#include "UI\UISlider.h"
#include "Unit\Unit.h"
#include "GameState.h"

void UIBattleSlider::onStart()
{
}

void UIBattleSlider::onUpdate()
{
	if (m_pPlayer == nullptr ||
		m_pEnemy == nullptr) return;

	//�v���C���[�̐�͒l�̌v�Z
	float playerBattleValue = 0.0f;
	auto& playerUnits = m_pPlayer->getUnitContainer()->getUnits();
	for (auto pUnit : playerUnits)
	{
		playerBattleValue += pUnit->getHealth() + pUnit->getShield();
	}

	//�G�̐�͒l�̌v�Z
	float enemyBattleValue = 0.0f;
	auto& enemyUnits = m_pEnemy->getUnitContainer()->getUnits();
	for (auto pUnit : enemyUnits)
	{
		enemyBattleValue += pUnit->getHealth() + pUnit->getShield();
	}

	//��͒l�̍��v���v�Z
	int battleValueSum = playerBattleValue + enemyBattleValue;

	m_pPlayerSlider->setMaxValue(battleValueSum);
	m_pEnemySlider->setMaxValue(battleValueSum);

	//���������Ȃ�ԐF�ɓ_��
	if ((float)playerBattleValue / (float)battleValueSum < 0.3f)
	{
		Color sliderColor(DirectX::Colors::Orange);
		float sin = std::fabsf(std::sinf(GameDevice::getGameTime().getUnscaledTotalTime() * 5.0f));
		sliderColor.r -= sin;
		sliderColor.g -= sin;
		sliderColor.b -= sin;
		m_pPlayerSlider->setColor(sliderColor);
	}
	else
	{
		m_pPlayerSlider->setColor(Color(DirectX::Colors::Orange));
	}

	//���������Ȃ甒���_��
	if ((float)enemyBattleValue / (float)battleValueSum < 0.3f)
	{
		Color sliderColor(DirectX::Colors::Red);
		float sin = std::fabsf(std::sinf(GameDevice::getGameTime().getUnscaledTotalTime() * 5.0f));
		sliderColor.r += sin;
		sliderColor.g += sin;
		sliderColor.b += sin;
		m_pEnemySlider->setColor(sliderColor);
	}
	else
	{
		m_pEnemySlider->setColor(Color(DirectX::Colors::Red));
	}

	//���ݒl��ݒ�
	m_pPlayerSlider->setCurrentValue(playerBattleValue);
	m_pEnemySlider->setCurrentValue(enemyBattleValue);
}

void UIBattleSlider::init(IPlayer* pPlayer, IPlayer* pEnemy)
{
	m_pPlayer = pPlayer;
	m_pEnemy = pEnemy;

	//�v���C���[���̐�͒l�\���X���C�_�[
	auto pPlayerSliderObj = new GameObject(getUser().getGameMediator());
	pPlayerSliderObj->setParent(&getUser());
	pPlayerSliderObj->getTransform().setLocalPosition(Vec3(0.0f, 300.0f, 1.0f));
	pPlayerSliderObj->getTransform().setLocalScale(Vec3(320.0f, 1.0f, 1.0f));
	m_pPlayerSlider = pPlayerSliderObj->addComponent<UISlider>();
	m_pPlayerSlider->setTextureByName("BoxFill");
	m_pPlayerSlider->setColor(Color(DirectX::Colors::Orange));
	m_pPlayerSlider->setDirection(UISlider::Direction::RIGHT);
	m_pPlayerSlider->setWidth(32.0f);
	m_pPlayerSlider->setMaxValue(1.0f);

	//�G���̐�͒l�\���X���C�_�[
	auto pEnemySliderObj = new GameObject(getUser().getGameMediator());
	pEnemySliderObj->setParent(&getUser());
	pEnemySliderObj->getTransform().setLocalPosition(Vec3(0.0f, 300.0f, 1.0f));
	pEnemySliderObj->getTransform().setLocalScale(Vec3(320.0f, 1.0f, 1.0f));
	m_pEnemySlider = pEnemySliderObj->addComponent<UISlider>();
	m_pEnemySlider->setTextureByName("BoxFill");
	m_pEnemySlider->setColor(Color(DirectX::Colors::Red));
	m_pEnemySlider->setDirection(UISlider::Direction::LEFT);
	m_pEnemySlider->setWidth(32.0f);
	m_pEnemySlider->setMaxValue(1.0f);
}