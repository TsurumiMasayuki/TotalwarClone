#pragma once
#include <Component/Physics/AbstractColliderBt.h>

class SphereColliderBt
	: public AbstractColliderBt
{
	// AbstractColliderBt ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void createCollisionShape() override;
};