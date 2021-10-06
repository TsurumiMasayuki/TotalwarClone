#pragma once
#include <wrl.h>
#include <vector>
#include <d3d12.h>
#include <DirectXMath.h>
#include <Utility\INonCopyable.h>

template<typename T>
class DX12VertexBuffer
	: private INonCopyable
{
public:
	void init(ID3D12Device * pDevice, const std::vector<T>& vertices);

	void setData(const std::vector<T>& vertices);

	UINT getVertexCount() const { return m_VertexCount; }
	const D3D12_VERTEX_BUFFER_VIEW& getVertexBufferView() const { return m_VertexBufferView; }

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> m_pResource;
	D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
	UINT m_VertexCount;
};

template<typename T>
void DX12VertexBuffer<T>::init(ID3D12Device * pDevice, const std::vector<T>& vertices)
{
	m_VertexCount = vertices.size();

	//ヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	ZeroMemory(&heapProp, sizeof(D3D12_HEAP_PROPERTIES));
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

	//リソース設定
	D3D12_RESOURCE_DESC resourceDesc{};
	ZeroMemory(&resourceDesc, sizeof(D3D12_RESOURCE_DESC));
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = sizeof(T) * m_VertexCount;	//頂点情報が入る分のサイズ
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//GPUリソースの生成
	HRESULT result = pDevice->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_pResource));

	//GPU上のバッファに対応した仮想メモリを取得
	T* pVertexMap = nullptr;
	result = m_pResource->Map(0, nullptr, (void**)&pVertexMap);
	//全頂点に対して適用
	for (UINT i = 0; i < m_VertexCount; ++i)
	{
		pVertexMap[i] = vertices[i];
	}
	//マップを解除
	m_pResource->Unmap(0, nullptr);

	//頂点バッファを作成
	m_VertexBufferView.BufferLocation = m_pResource->GetGPUVirtualAddress();
	m_VertexBufferView.SizeInBytes = sizeof(T) * m_VertexCount;
	m_VertexBufferView.StrideInBytes = sizeof(T);
}

template<typename T>
void DX12VertexBuffer<T>::setData(const std::vector<T>& vertices)
{
	//GPU上のバッファに対応した仮想メモリを取得
	T* pVertexMap = nullptr;
	HRESULT result = m_pResource->Map(0, nullptr, (void**)&pVertexMap);
	//全頂点に対して適用
	for (int i = 0; i < m_VertexCount; ++i)
	{
		pVertexMap[i] = vertices[i];
	}
	//マップを解除
	m_pResource->Unmap(0, nullptr);
}
