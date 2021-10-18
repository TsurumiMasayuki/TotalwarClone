#include "BlockbenchModel.h"

BlockbenchModel::BlockbenchModel(const std::vector<DirectX::XMMATRIX>& cubeMatrices)
{
	//vector‚Ìƒƒ‚ƒŠŠm•Û
	m_CubeMatrices.reserve(cubeMatrices.size());

	//s—ñ‚ğˆÚ‚µ‘Ö‚¦‚é
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
