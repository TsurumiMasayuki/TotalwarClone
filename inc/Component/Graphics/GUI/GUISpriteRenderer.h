#pragma once
#include <Component\Graphics\SpriteRenderer.h>

class GUISpriteRenderer
	: public SpriteRenderer
{
public:
	virtual void draw(Camera* pCamera, ID3D12GraphicsCommandList* pCommandList) const override;
};