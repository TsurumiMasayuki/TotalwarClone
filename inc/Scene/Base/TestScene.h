#pragma once

#include <Graphics\DX12\DX12RenderTarget.h>
#include <Scene\Base\AbstractScene.h>

class AbstractMaterial;

class TestScene
	: public AbstractScene
{
public:
	virtual std::string nextScene() override;
	virtual bool isEnd() override;

	virtual void start() override;
	virtual void update() override;
	virtual void shutdown() override;

private:
	AbstractMaterial* m_pMaterial;
};