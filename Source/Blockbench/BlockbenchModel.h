#pragma once
#include <vector>
#include <DirectXMath.h>

#include "Math\Vec3.h"

class BlockbenchModel
{
public:
	BlockbenchModel(const std::vector<DirectX::XMMATRIX>& cubeMatrices);

	const std::vector<DirectX::XMMATRIX>& getCubeMatrices() const;

private:
	//—§•û‘Ì‚Ìs—ñ
	std::vector<DirectX::XMMATRIX> m_CubeMatrices;
};