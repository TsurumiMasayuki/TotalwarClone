#pragma once
#include <Component\Base\AbstractComponent.h>
#include <Component\Graphics\IRenderer.h>

#include <Utility\Color.h>
#include <Utility\Timer.h>

#include <DirectXMath.h>

class DX12Texture2D;
class ParticleMaterial;

struct ParticleVertex
{
	DirectX::XMFLOAT3 pos;
};

//TODO
//・パーティクルクラスのvectorを作ってupdateで更新
//・パーティクルごとにConstantBufferの中身を差し替え(DirectX12の描画は"コマンド"を後から実行するかもなので、ConstantBufferをパーティクル毎に用意しないと失敗するかも)
class ParticleRenderer
	: public AbstractComponent, public IRenderer
{
private:
	class Particle
	{
	public:
		Particle();

		void update();
		DirectX::XMMATRIX getWorldMatrix();

	public:
		Vec3 m_Pos;
		float m_AngleZ;
		float m_AngleVelocity;
		Vec3 m_Scale;
		Vec3 m_Velocity;
		Color m_Color;

	private:
		Timer m_LifeTimer;
	};

public:
	ParticleRenderer();
	~ParticleRenderer();

	virtual void onStart() override;
	virtual void onUpdate() override;

	virtual void setDrawOrder(int drawOrder) override;
	virtual int getDrawOrder() const override;

	virtual const CullingInfo& getCullingInfo() const override;
	virtual void draw(Camera* pCamera, ID3D12GraphicsCommandList* pCommandList) const override;

	const std::string& getTextureName() const { return m_TextureName; }
	void setTextureByName(const std::string& key);

private:
	virtual void setColor(const Color& color) override;
	virtual const Color& getColor() const override;

private:
	CullingInfo m_CullingInfo;
	Color m_DummyColor;
	ParticleMaterial* m_pMaterial;

	std::string m_TextureName;
	DX12Texture2D* m_pTexture2D;
};