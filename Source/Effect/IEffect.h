#pragma once

class IEffect
{
public:
	virtual ~IEffect() {};

	virtual void playEffect() = 0;

protected:
	IEffect() {};
};