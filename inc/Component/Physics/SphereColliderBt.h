#pragma once
#include <Component/Physics/AbstractColliderBt.h>

class SphereColliderBt
	: public AbstractColliderBt
{
	// AbstractColliderBt ����Čp������܂���
	virtual void createCollisionShape() override;
};