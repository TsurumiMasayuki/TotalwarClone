#pragma once
#include <vector>
#include "Component\Base\AbstractComponent.h"

namespace Action
{
	class ActionManager;
}

class Transform;

class UIStageList
	: public AbstractComponent
{
public:
	virtual void onStart() override;
	virtual void onUpdate() override;

	const std::string& getSelectedStageName() const;

private:
	void shiftStageCard(float direction);

private:
	int m_CurrentIndex;
	std::vector<std::string> m_StageNameList;
	std::vector<Transform*> m_StageCardList;
	std::vector<Action::ActionManager*> m_ActionManagers;
};