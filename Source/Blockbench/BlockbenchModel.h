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

//キューブの情報UV保存用構造体
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

//Blockbenchモデル保持用クラス
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
	//マテリアル
	AbstractMaterial* m_pMaterial;

	//UVの行列
	std::vector<Cube> m_Cubes;

	//テクスチャ名
	std::string m_TextureName;

	//Cubeの数
	unsigned int m_CubeCount;
};