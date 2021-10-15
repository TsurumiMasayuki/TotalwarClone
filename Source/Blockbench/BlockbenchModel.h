#pragma once
#include <vector>
#include <DirectXMath.h>

#include "Math\Vec3.h"

class BlockbenchModel
{
public:
	BlockbenchModel(const std::vector<DirectX::XMMATRIX>& cubeMatrices);

	const std::vector<DirectX::XMFLOAT4X4>& getCubeMatrices() const;

private:
	//立方体の行列
	std::vector<DirectX::XMFLOAT4X4> m_CubeMatrices;
};