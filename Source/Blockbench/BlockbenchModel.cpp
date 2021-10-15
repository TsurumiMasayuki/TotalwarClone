#include "BlockbenchModel.h"

BlockbenchModel::BlockbenchModel(const std::vector<DirectX::XMMATRIX>& cubeMatrices)
{
	//vectorのメモリ確保
	m_CubeMatrices.reserve(cubeMatrices.size());

	//行列を移し替える
	for (auto& matrix : cubeMatrices)
	{
		m_CubeMatrices.emplace_back();
		DirectX::XMStoreFloat4x4(&m_CubeMatrices.back(), DirectX::XMMatrixTranspose(matrix));
	}
}

const std::vector<DirectX::XMFLOAT4X4>& BlockbenchModel::getCubeMatrices() const
{
	return m_CubeMatrices;
}
