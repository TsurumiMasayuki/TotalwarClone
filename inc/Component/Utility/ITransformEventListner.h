#pragma once
#include <Math\Vec3.h>

class ITransformEventListner
{
public:
	virtual ~ITransformEventListner() {}

	virtual void onLocalPositionChanged(const Vec3& newLocalPosition) = 0;
	virtual void onLocalScaleChanged(const Vec3& newLocalScale) = 0;
	virtual void onLocalAnglesChanged(const Vec3& newLocalAngle) = 0;

protected:
	ITransformEventListner() {}
};
