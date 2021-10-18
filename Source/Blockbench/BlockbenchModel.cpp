#include "BlockbenchModel.h"

BlockbenchModel::BlockbenchModel(const std::vector<DirectX::XMMATRIX>& cubeMatrices)
{
	//vectorのメモリ確保
	m_CubeMatrices.reserve(cubeMatrices.size());

	//行列を移し替える
	for (auto& matrix : cubeMatrices)
	{
		m_CubeMatrices.emplace_back();
		m_CubeMatrices.back() = matrix;
	}
}

const std::vector<DirectX::XMMATRIX>& BlockbenchModel::getCubeMatrices() const
{
	return m_CubeMatrices;
}
