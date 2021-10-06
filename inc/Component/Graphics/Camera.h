#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <Component\Base\AbstractComponent.h>
#include <Graphics\DX12\DX12RenderTarget.h>

enum class DrawMode
{
	DrawMode_2D,
	DrawMode_3D
};

class Camera
	: public AbstractComponent
{
public:

	Camera();
	~Camera();

	virtual void onStart() override;
	virtual void onUpdate() override;

	virtual void onLocalPositionChanged(const Vec3& newLocalPosition) override { updateCameraMatrix(); }
	virtual void onLocalAnglesChanged(const Vec3& newLocalAngles) override { updateCameraMatrix(); }

	int getDrawOrder();
	void setRenderTarget(DX12RenderTarget* pRenderTarget);
	DX12RenderTarget* getRenderTarget();

	void setDrawMode(DrawMode drawMode);

	const DirectX::XMMATRIX& getViewMatrix() { return m_ViewMatrix; };
	const DirectX::XMMATRIX& getProjMatrix() { return m_ProjMatrix; };
	const DirectX::XMMATRIX& getBillboardMatrix() { return m_Billboard; }

private:
	void updateCameraMatrix();

private:
	int m_DrawOrder;
	float m_Fov;

	DX12RenderTarget* m_pRenderTarget;

	DirectX::XMMATRIX m_ViewMatrix;
	DirectX::XMMATRIX m_ProjMatrix;
	DirectX::XMMATRIX m_Billboard;
};