#pragma once
#include <DirectXMath.h>

struct UnitInstanceInfo
{
	DirectX::XMFLOAT4X4 instanceMat;
	DirectX::XMFLOAT4 instanceColor;
	//XY, XY�œ����
	DirectX::XMFLOAT4X4 instanceUVOrigins;
	//XY, XY�œ����
	DirectX::XMFLOAT4X4 instanceUVSizes;
};