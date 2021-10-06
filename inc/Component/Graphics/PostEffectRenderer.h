#pragma once
#include <Component\Graphics\SpriteRenderer.h>

class AbstractMaterial;

class PostEffectRenderer
	: public SpriteRenderer
{
public:
	PostEffectRenderer();
	~PostEffectRenderer();

	void setMaterial(AbstractMaterial* pMaterial);
	virtual void draw(Camera* pCamera, ID3D12GraphicsCommandList* pCommandList) const override;

private:
	AbstractMaterial* m_pMaterial;
};