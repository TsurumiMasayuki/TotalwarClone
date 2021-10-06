#pragma once
#include <DirectXMath.h>
#include <Component\Base\AbstractComponent.h>
#include <Component\Graphics\IRenderer.h>
#include <Graphics\DX12\Material\SimpleMaterials.h>
#include <Graphics\DX12\DX12VertexBuffer.h>
#include <Graphics\DX12\DX12IndexBuffer.h>
#include <Utility\Color.h>

struct SimpleVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
};

class CircleRenderer
	: public AbstractComponent, public IRenderer
{
public:
	CircleRenderer();
	~CircleRenderer();

	virtual void onStart() override;
	virtual void onUpdate() override;

	virtual void setDrawOrder(int drawOrder) override { m_DrawOrder = drawOrder; }
	virtual int getDrawOrder() const override { return m_DrawOrder; }

	virtual void setColor(const Color & color) override;
	virtual const Color & getColor() const override;
	virtual const CullingInfo & getCullingInfo() const override;
	virtual void draw(Camera * pCamera, ID3D12GraphicsCommandList * pCommandList) const override;

private:
	void initBuffers();

private:
	int m_DrawOrder;
	CullingInfo m_CullingInfo;
	CircleMaterial* m_pMaterial;
	Color m_Color;

	static DX12VertexBuffer<SimpleVertex> g_VertexBuffer;
	static DX12IndexBuffer g_IndexBuffer;
};