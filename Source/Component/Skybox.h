#pragma once
#include <array>
#include "Component\Base\AbstractComponent.h"

class Skybox
	: public AbstractComponent
{
public:
	virtual void onStart() override;
	virtual void onUpdate() override;
};