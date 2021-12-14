#include "UICombatStatus.h"

#include "Actor\Base\GameObject.h"
#include "Component\Graphics\GUI\GUISpriteRenderer.h"
#include "Math\Vec3.h"

#include "Unit\Unit.h"
#include "Unit\UnitStats.h"

void UICombatStatus::onStart()
{
}

void UICombatStatus::onUpdate()
{
	Unit* pTarget = m_pUnit->getTarget();

	//ターゲットが無いor自身が全滅状態なら
	if (pTarget == nullptr ||
		m_pUnit->getObjectCount() == 0)
	{
		//非表示にする
		m_pRenderer->setColor(Color(0.0f, 0.0f, 0.0f, 0.0f));
		return;
	}

	//ユニットのHP
	float unitHealth = m_pUnit->getHealth();
	//ターゲットのHP
	float targetHealth = pTarget->getHealth();

	float diff = std::fabsf(unitHealth - targetHealth);

	Color color;
	//体力を比較して拮抗状態ならオレンジ色
	if (diff < 500.0f)
	{
		color = Color(DirectX::Colors::Orange);
	}

	//体力を比較してこちら側が不利なら赤色
	if (unitHealth < targetHealth)
	{
		color = Color(DirectX::Colors::Red);
	}

	//体力を比較してこちら側が有利なら赤色
	if (unitHealth > targetHealth)
	{
		color = Color(DirectX::Colors::Green);
	}

	//色を設定
	m_pRenderer->setColor(color);
}

void UICombatStatus::init(Unit* pUnit)
{
	m_pUnit = pUnit;

	m_pRenderer = getUser().addComponent<GUISpriteRenderer>();
	m_pRenderer->setTextureByName("CombatIcon");
	//非表示にする
	m_pRenderer->setColor(Color(0.0f, 0.0f, 0.0f, 0.0f));
}
