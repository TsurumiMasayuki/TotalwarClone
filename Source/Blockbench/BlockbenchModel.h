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

//�L���[�u�̏��UV�ۑ��p�\����
struct Cube
{
	Cube()
		: m_Matrix(DirectX::XMMatrixIdentity())
	{

	}

	Cube(const UV uvArray[36], 
		const DirectX::XMMATRIX& matrix)
	{
		for (int i = 0; i < 36; i++)
		{
			m_UVArray[i] = uvArray[i];
		}

		m_Matrix = matrix;
	}

	UV m_UVArray[36];
	DirectX::XMMATRIX m_Matrix;
};

//Blockbench���f���ێ��p�N���X
class BlockbenchModel
{
public:
	static const int cubeFaceCount = 6;

public:
	BlockbenchModel(const std::vector<Cube>& cubes,
		const std::string& textureName,
		unsigned int cubeCount);

	const std::string& getTextureName() const;
	const std::vector<Cube>& getCubes() const;

	unsigned int getCubeCount() const;

private:
	//�}�e���A��
	AbstractMaterial* m_pMaterial;

	//UV�̍s��
	std::vector<Cube> m_Cubes;

	//�e�N�X�`����
	std::string m_TextureName;

	//Cube�̐�
	unsigned int m_CubeCount;
};