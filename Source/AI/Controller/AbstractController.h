#pragma once
#include "AI\ValueMap.h"
#include "Unit\Unit.h"
#include "Player\IPlayer.h"

class AbstractController
{
public:
	virtual ~AbstractController() {};

	virtual void start() = 0;
	virtual void update() = 0;

protected:
	AbstractController() {};
};