#pragma once
#include <Component\Graphics\CullingInfo.h>
#include <Utility\Color.h>

struct ID3D12GraphicsCommandList;
class Camera;

class IRenderer
{
public:
	virtual ~IRenderer() {}

	virtual void setDrawOrder(int drawOrder) = 0;
	virtual int getDrawOrder() const = 0;

	virtual void setColor(const Color& color) = 0;
	virtual const Color& getColor() const = 0;

	virtual const CullingInfo& getCullingInfo() const = 0;
	virtual void draw(Camera* pCamera, ID3D12GraphicsCommandList * pCommandList) const = 0;

protected:
	IRenderer() {}
};