#pragma once
#include <string>
#include <vector>
#include <DirectXMath.h>

class AbstractMaterial;

struct UV
{
	UV()
		: x(0.0f),
		y(0.0f)
	{
	}

	UV(float x, float y)
		: x(x),
		y(y)
	{
	}

	float x;
	float y;
};

//Blockbench���f���ێ��p�N���X
class BlockbenchModel
{
public:
	static const int cubeFaceCount = 6;

public:
	BlockbenchModel(const std::vector<DirectX::XMMATRIX>& cubeMatrices,
		const std::vector<UV>& uvOrigins,
		const std::vector<UV>& uvSizes,
		const std::string& textureName);

	const std::string& getTextureName() const;
	const std::vector<DirectX::XMMATRIX>& getCubeMatrices() const;
	const std::vector<UV>& getUVOrigins() const;
	const std::vector<UV>& getUVSizes() const;

private:
	//�}�e���A��
	AbstractMaterial* m_pMaterial;

	//�����̂̍s��
	std::vector<DirectX::XMMATRIX> m_CubeMatrices;
	//UV�̍s��
	std::vector<UV> m_CubeUVOrigins;
	//UV�̍s��
	std::vector<UV> m_CubeUVSizes;

	//�e�N�X�`����
	std::string m_TextureName;
};