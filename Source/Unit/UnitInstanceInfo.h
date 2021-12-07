#pragma once
#include <DirectXMath.h>

struct UnitInstanceInfo
{
	DirectX::XMFLOAT4X4 instanceMat;
	DirectX::XMFLOAT4 instanceColor;
	//XY, XY‚Å“ü‚ê‚é
	DirectX::XMFLOAT3X4 instanceUVOrigins;
	//XY, XY‚Å“ü‚ê‚é
	DirectX::XMFLOAT3X4 instanceUVSizes;
};