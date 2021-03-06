#pragma once
#include <DirectXMath.h>

struct UnitInstanceInfo
{
	DirectX::XMFLOAT4X4 instanceMat;
	DirectX::XMFLOAT4 instanceColor;
	//XY, XYで入れる
	DirectX::XMFLOAT3X4 instanceUVOrigins;
	//XY, XYで入れる
	DirectX::XMFLOAT3X4 instanceUVSizes;
};