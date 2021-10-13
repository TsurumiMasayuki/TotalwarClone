#pragma once

#include "Component\Base\AbstractComponent.h"

class Camera;

class Cursor
	: public AbstractComponent
{
public:
	void init(Camera* pCamera);

	virtual void onStart() override;
	virtual void onUpdate() override;

	const Vec3& getCursorPoint() const;

private:
	Vec3 m_CursorRayPoint;
	Camera* m_pCamera;
};