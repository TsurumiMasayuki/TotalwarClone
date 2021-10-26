#include "BlockbenchModel.h"

BlockbenchModel::BlockbenchModel(const std::vector<DirectX::XMMATRIX>& cubeMatrices,
	const std::vector<UV>& uvOrigins,
	const std::vector<UV>& uvSizes,
	const std::string& textureName)
	: m_TextureName(textureName)
{
	//vector‚Ìƒƒ‚ƒŠŠm•Û
	m_CubeMatrices.reserve(cubeMatrices.size());
	m_CubeUVOrigins.reserve(uvOrigins.size());
	m_CubeUVSizes.reserve(uvSizes.size());

	//s—ñ‚ğˆÚ‚µ‘Ö‚¦‚é
	for (auto& matrix : cubeMatrices)
	{
		m_CubeMatrices.emplace_back();
		m_CubeMatrices.back() = matrix;
	}

	//UV‚ğˆÚ‚µ‘Ö‚¦‚é
	for (auto& uvOrigin : uvOrigins)
	{
		m_CubeUVOrigins.emplace_back();
		m_CubeUVOrigins.back() = uvOrigin;
	}

	for (auto& uvSize : uvSizes)
	{
		m_CubeUVSizes.emplace_back();
		m_CubeUVSizes.back() = uvSize;
	}
}

const std::string& BlockbenchModel::getTextureName() const
{
	return m_TextureName;
}

const std::vector<DirectX::XMMATRIX>& BlockbenchModel::getCubeMatrices() const
{
	return m_CubeMatrices;
}

const std::vector<UV>& BlockbenchModel::getUVOrigins() const
{
	return m_CubeUVOrigins;
}

const std::vector<UV>& BlockbenchModel::getUVSizes() const
{
	return m_CubeUVSizes;
}
