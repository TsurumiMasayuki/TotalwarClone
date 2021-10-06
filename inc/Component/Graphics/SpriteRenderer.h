#pragma once
#include <string>
#include <DirectXMath.h>
#include <Component\Base\AbstractComponent.h>
#include <Component\Graphics\IRenderer.h>
#include <Graphics\DX12\DX12VertexBuffer.h>
#include <Graphics\DX12\DX12IndexBuffer.h>
#include <Utility\Color.h>

//TODO 
//ピボット機能の追加 
//カリングの実装

class DX12Texture2D;
class SpriteMaterial;

//スプライト用頂点
struct SpriteVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
};

class SpriteRenderer
	: public AbstractComponent, public IRenderer
{
public:
	SpriteRenderer();
	virtual ~SpriteRenderer();

	virtual void onStart() override;
	virtual void onUpdate() override;

	virtual void setDrawOrder(int drawOrder) override { m_DrawOrder = drawOrder; }
	virtual int getDrawOrder() const override { return m_DrawOrder; }

	virtual void setColor(const Color& color) override { m_Color = color; }
	virtual const Color& getColor() const override { return m_Color; }

	virtual const CullingInfo & getCullingInfo() const override;
	virtual void draw(Camera * pCamera, ID3D12GraphicsCommandList * pCommandList) const override;

	void setTexture(DX12Texture2D* pTexture2D);
	void setTextureByName(const std::string& key);

private:
	void initBuffers();

protected:
	int m_DrawOrder;
	CullingInfo m_CullingInfo;
	SpriteMaterial* m_pMaterial;
	Color m_Color;

	static DX12VertexBuffer<SpriteVertex> vertexBuffer;
	static DX12IndexBuffer indexBuffer;
};