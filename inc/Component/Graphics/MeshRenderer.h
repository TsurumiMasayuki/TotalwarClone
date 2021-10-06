#pragma once
#include <string>
#include <DirectXMath.h>
#include <Component\Base\AbstractComponent.h>
#include <Component\Graphics\IRenderer.h>
#include <Device\Resource\DX12Mesh.h>
#include <Graphics\DX12\DX12Texture2D.h>
#include <Utility\Color.h>

#include <fbxsdk.h>

class MeshMaterial;

//TODO 
//�J�����O�̎���

class MeshRenderer
	: public AbstractComponent, public IRenderer
{
public:
	MeshRenderer();
	~MeshRenderer();

	virtual void onStart() override;
	virtual void onUpdate() override;

	virtual void setDrawOrder(int drawOrder) override { m_DrawOrder = drawOrder; }
	virtual int getDrawOrder() const override { return m_DrawOrder; }

	virtual void setColor(const Color& color) override;
	virtual const Color& getColor() const override;

	virtual const CullingInfo & getCullingInfo() const override;
	virtual void draw(Camera * pCamera, ID3D12GraphicsCommandList * pCommandList) const override;

	const DX12Mesh* getMesh() const;
	void setMesh(DX12Mesh* pMesh) { m_pMesh = pMesh; }

	void setTexture(DX12Texture2D* pTexture2D);
	void setTextureByName(const std::string& key);
	const std::string& getTextureName() const { return m_TextureName; }

	void setMetalness(float metalness);
	void setSpecular(float specular);
	void setRoughness(float roughness);

	void playAnimation();

private:
	int m_DrawOrder;
	CullingInfo m_CullingInfo;

	std::string m_TextureName;

	DX12Mesh* m_pMesh;
	MeshMaterial* m_pMaterial;

	//1�t���[���̎���
	fbxsdk::FbxTime m_FrameTime;
	//�A�j���[�V�����J�n����
	fbxsdk::FbxTime m_StartTime;
	//�A�j���[�V�����I������
	fbxsdk::FbxTime m_EndTime;
	//���ݎ���(�A�j���[�V����)
	fbxsdk::FbxTime m_CurrentTime;
	//�A�j���[�V�����Đ���
	bool m_IsPlay = false;
};