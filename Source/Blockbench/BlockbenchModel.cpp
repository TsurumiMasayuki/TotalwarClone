#include "BlockbenchModel.h"

BlockbenchModel::BlockbenchModel(const std::vector<Cube>& cubes,
	const std::string& textureName,
	unsigned int cubeCount)
	: m_TextureName(textureName),
	m_CubeCount(cubeCount)
{
	m_Cubes.reserve(cubes.size());

	//ƒLƒ…[ƒuî•ñ‚ğˆÚ‚µ‘Ö‚¦‚é
	for (auto& cube : cubes)
	{
		m_Cubes.emplace_back();
		m_Cubes.back() = cube;
	}
}

const std::string& BlockbenchModel::getTextureName() const
{
	return m_TextureName;
}

const std::vector<Cube>& BlockbenchModel::getCubes() const
{
	return m_Cubes;
}

unsigned int BlockbenchModel::getCubeCount() const
{
	return m_CubeCount;
}
