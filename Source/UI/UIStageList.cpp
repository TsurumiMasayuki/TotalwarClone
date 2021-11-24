#include "UIStageList.h"
#include "Actor\Base\GameObject.h"
#include "Device\GameDevice.h"

#include "Stage\Stage.h"
#include "UI\UIStageCard.h"
#include "Utility\JsonFileManager.h"

#include "Math\MathUtility.h"

const float g_StageCardWidth = 1280.0f / 4;
const float g_StageCardHeight = 720.0f / 4;

void UIStageList::onStart()
{
	m_CurrentIndex = 0;

	const auto& stageFiles = JsonFileManager<Stage>::getInstance().getAll();

	int count = 0;
	//ステージを列挙して並べる
	for (auto& pair : stageFiles)
	{
		auto pObj = new GameObject(getUser().getGameMediator());
		auto& tr = pObj->getTransform();
		tr.setLocalPosition(Vec3(g_StageCardWidth * count, 0.0f, 1.0f));
		tr.setLocalScale(Vec3(1.0f));
		m_StageCardList.emplace_back(&tr);
		auto pStageCard = pObj->addComponent<UIStageCard>();
		pStageCard->setStage(pair.first);
		m_StageNameList.emplace_back(pair.first);

		count++;
	}
}

void UIStageList::onUpdate()
{
	if (GameDevice::getInput().isKeyDown(DIK_LEFTARROW))
	{
		shiftStageCard(-1.0f);
	}

	if (GameDevice::getInput().isKeyDown(DIK_RIGHTARROW))
	{
		shiftStageCard(1.0f);
	}
}

const std::string& UIStageList::getSelectedStageName() const
{
	return m_StageNameList.at(m_CurrentIndex);
}

void UIStageList::shiftStageCard(float direction)
{
	//範囲内か判定
	int newIndex = m_CurrentIndex + (int)direction;
	if (!MathUtility::isInRange((float)newIndex, 0.0f, (float)m_StageNameList.size() - 1.0f))
		return;

	m_CurrentIndex = newIndex;

	//ステージ表示をずらす
	int count = 0;
	for (auto tr : m_StageCardList)
	{
		const Vec3& position = tr->getLocalPosition();
		tr->setLocalPosition(position + Vec3(g_StageCardWidth * -direction, 0.0f, 0.0f));
		count++;
	}
}
