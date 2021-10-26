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

//Blockbenchモデル保持用クラス
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
	//マテリアル
	AbstractMaterial* m_pMaterial;

	//立方体の行列
	std::vector<DirectX::XMMATRIX> m_CubeMatrices;
	//UVの行列
	std::vector<UV> m_CubeUVOrigins;
	//UVの行列
	std::vector<UV> m_CubeUVSizes;

	//テクスチャ名
	std::string m_TextureName;
};