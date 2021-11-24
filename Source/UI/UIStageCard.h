#pragma once
#include <string>
#include "Component\Base\AbstractComponent.h"

class D2DTextRenderer;

class UIStageCard
	: public AbstractComponent
{
public:

	virtual void onStart() override;
	virtual void onUpdate() override;

	virtual void onLocalPositionChanged(const Vec3& newPosition) override;

	void setStage(const std::string& stageName);

	const std::string& getStageName() const;

private:
	D2DTextRenderer* m_pStageNameRenderer;
};