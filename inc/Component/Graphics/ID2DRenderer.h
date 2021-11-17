#pragma once

class ID2D1DeviceContext2;

class ID2DRenderer
{
public:
	virtual ~ID2DRenderer() {}
	virtual void draw(ID2D1DeviceContext2* pDeviceContext) = 0;

protected:
	ID2DRenderer() {}
};