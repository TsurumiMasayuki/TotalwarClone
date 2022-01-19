#pragma once
#include <DirectXMath.h>
#include <Actor\Base\GameObject.h>
#include <Component\Base\AbstractComponent.h>
#include <Component\Graphics\IRenderer.h>
#include <Device\Resource\DX12Mesh.h>
#include <Device\GameDevice.h>
#include <Graphics\DX12\DX12Texture2D.h>
#include <Graphics\DX12\Material\AbstractMaterial.h>
#include <Utility\Color.h>

#include <Graphics\DX12\DX12VertexBuffer.h>

template<typename T>
class InstancedRenderer
	: public AbstractComponent, public IRenderer
{
public:
	InstancedRenderer();
	~InstancedRenderer();

	virtual void onStart() override;
	virtual void onUpdate() override;

	virtual void setDrawOrder(int drawOrder) override;
	virtual int getDrawOrder() const override;
	virtual const CullingInfo& getCullingInfo() const override;
	virtual void draw(Camera* pCamera, ID3D12GraphicsCommandList* pCommandList) const override;

	const DX12Mesh* getMesh() const;
	void setMesh(DX12Mesh* pMesh);

	void setMaterial(AbstractMaterial* pMaterial);

	void setTexture(DX12Texture2D* pTexture2D);
	void setTextureByName(const std::string& key);

	void setInstanceInfo(const std::vector<T>& instanceInfo);
	void setInstanceCount(int count);

private:
	virtual void setColor(const Color& color) override;
	virtual const Color& getColor() const override;

private:
	DX12Mesh* m_pMesh;
	AbstractMaterial* m_pMaterial;
	DX12Texture2D* m_pTexture2D;

	Color m_Color;
	DX12VertexBuffer<T>* m_pInstanceBuffer;

	int m_InstanceCount;
};

template<typename T>
InstancedRenderer<T>::InstancedRenderer()
	: m_Color(DirectX::Colors::White),
	m_pMesh(nullptr),
	m_pTexture2D(nullptr),
	m_pInstanceBuffer(nullptr),
	m_InstanceCount(0)
{
	GameDevice::getRenderManager().addRenderer(this);
}

template<typename T>
InstancedRenderer<T>::~InstancedRenderer()
{
	GameDevice::getRenderManager().removeRenderer(this);
}

template<typename T>
void InstancedRenderer<T>::onStart()
{
}

template<typename T>
inline void InstancedRenderer<T>::onUpdate()
{
}

template <typename T>
void InstancedRenderer<T>::setDrawOrder(int drawOrder)
{
}

template <typename T>
int InstancedRenderer<T>::getDrawOrder() const
{
	return 100;
}

template <typename T>
void InstancedRenderer<T>::setColor(const Color& color)
{
}

template <typename T>
const Color& InstancedRenderer<T>::getColor() const
{
	return m_Color;
}

template <typename T>
const CullingInfo& InstancedRenderer<T>::getCullingInfo() const
{
	return CullingInfo(Vec3(-1.0f), Vec3(1.0f));
}

template <typename T>
void InstancedRenderer<T>::draw(Camera* pCamera, ID3D12GraphicsCommandList* pCommandList) const
{
	//インスタンスが無いなら描画しない
	if (m_InstanceCount == 0) return;

	//マテリアルが設定されていないなら描画しない
	if (m_pMaterial == nullptr) return;

	m_pMaterial->setMaterial(pCommandList);
	//WVP定数バッファを更新
	AbstractMaterial::DefaultBuffer* pMapWVP = nullptr;
	m_pMaterial->m_pDefaultConstantBuffer->map(0, nullptr, &pMapWVP);

	DirectX::XMMATRIX world = m_pUser->getTransform().getWorldMatrix();

	DirectX::XMMATRIX worldInv = DirectX::XMMatrixInverse(nullptr, world);
	worldInv = DirectX::XMMatrixTranspose(worldInv);

	DirectX::XMMATRIX wvpMatrix = world * pCamera->getViewMatrix() * pCamera->getProjMatrix();
	wvpMatrix = DirectX::XMMatrixTranspose(wvpMatrix);

	world = DirectX::XMMatrixTranspose(world);

	DirectX::XMStoreFloat4x4(&pMapWVP->wvp, wvpMatrix);
	DirectX::XMStoreFloat4x4(&pMapWVP->world, world);
	DirectX::XMStoreFloat4x4(&pMapWVP->worldInv, worldInv);
	DirectX::XMStoreFloat4x4(&pMapWVP->view, DirectX::XMMatrixTranspose(pCamera->getViewMatrix()));
	DirectX::XMStoreFloat4x4(&pMapWVP->projection, DirectX::XMMatrixTranspose(pCamera->getProjMatrix()));

	m_pMaterial->m_pDefaultConstantBuffer->unMap(0, nullptr);

	pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (m_pInstanceBuffer == nullptr)
	{
		D3D12_VERTEX_BUFFER_VIEW vbv[1] = { m_pMesh->getVertexBuffer().getVertexBufferView() };
		pCommandList->IASetVertexBuffers(0, _countof(vbv), vbv);
	}
	else
	{
		D3D12_VERTEX_BUFFER_VIEW vbv[2] = { m_pMesh->getVertexBuffer().getVertexBufferView(), m_pInstanceBuffer->getVertexBufferView() };
		pCommandList->IASetVertexBuffers(0, _countof(vbv), vbv);
	}

	pCommandList->IASetIndexBuffer(&m_pMesh->getIndexBuffer().getIndexBufferView());
	pCommandList->DrawIndexedInstanced(m_pMesh->getIndexBuffer().getIndexCount(), m_InstanceCount, 0, 0, 0);
}

template <typename T>
const DX12Mesh* InstancedRenderer<T>::getMesh() const
{
	return m_pMesh;
}

template <typename T>
void InstancedRenderer<T>::setMesh(DX12Mesh* pMesh)
{
	m_pMesh = pMesh;
}

template<typename T>
inline void InstancedRenderer<T>::setMaterial(AbstractMaterial* pMaterial)
{
	m_pMaterial = pMaterial;
}

template <typename T>
void InstancedRenderer<T>::setTexture(DX12Texture2D* pTexture2D)
{
	m_pTexture2D = pTexture2D;
}

template <typename T>
void InstancedRenderer<T>::setTextureByName(const std::string& key)
{
	if (key.empty()) return;

	m_pTexture2D = GameDevice::getTextureManager().getTexture(key);
}

template <typename T>
void InstancedRenderer<T>::setInstanceInfo(const std::vector<T>& instanceInfo)
{
	if (instanceInfo.size() == 0)
	{
		m_InstanceCount = 0;
		return;
	}
	//データの数が同じなら再利用
	bool reuseBuffer = m_InstanceCount == (int)instanceInfo.size();

	if (reuseBuffer)
	{
		m_pInstanceBuffer->setData(instanceInfo);
	}
	else
	{
		//既に生成されているなら削除
		if (m_pInstanceBuffer != nullptr)
			delete m_pInstanceBuffer;

		m_pInstanceBuffer = new DX12VertexBuffer<T>();
		m_pInstanceBuffer->init(DX12GraphicsCore::g_pDevice.Get(), instanceInfo);
		m_InstanceCount = (int)instanceInfo.size();
	}
}

template<typename T>
inline void InstancedRenderer<T>::setInstanceCount(int count)
{
	m_InstanceCount = count;
}