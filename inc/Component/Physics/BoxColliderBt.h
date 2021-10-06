#pragma once
#include <Component\Physics\AbstractColliderBt.h>

class BoxColiiderBt
	: public AbstractColliderBt
{
public:
	virtual void createCollisionShape() override;
};