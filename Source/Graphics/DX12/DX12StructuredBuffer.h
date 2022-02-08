#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <Utility\INonCopyable.h>
#include <vector>

template<typename T>
class DX12StructuredBuffer
	: public INonCopyable
{
public:
	void init(ID3D12Device* pDevice, const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, size_t bufferSize, UINT numElements, UINT firstElement = 0);

	void map(UINT subResource, D3D12_RANGE* pReadRange, T** pMap);
	void unMap(UINT subResource, D3D12_RANGE* pWrittenRange);

	D3D12_GPU_VIRTUAL_ADDRESS getGPUVirtualAddress() const { return m_pResource->GetGPUVirtualAddress(); }

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> m_pResource;
};

template<typename T>
inline void DX12StructuredBuffer<T>::init(ID3D12Device* pDevice,
	const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle,
	size_t bufferSize,
	UINT numElements,
	UINT firstElement)
{
	//リソースが作成済みだったらリリース
	if (m_pResource.Get() != nullptr)
		m_pResource->Release();

	//ヒーププロパティの設定
	D3D12_HEAP_PROPERTIES sbHeapProperties;
	sbHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	sbHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	sbHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	sbHeapProperties.CreationNodeMask = 1;
	sbHeapProperties.VisibleNodeMask = 1;

	//リソースの設定
	D3D12_RESOURCE_DESC sbResourceDesc;
	sbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	sbResourceDesc.Alignment = 0;
	sbResourceDesc.Width = bufferSize;
	sbResourceDesc.Height = 1;
	sbResourceDesc.DepthOrArraySize = 1;
	sbResourceDesc.MipLevels = 1;
	sbResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	sbResourceDesc.SampleDesc.Count = 1;
	sbResourceDesc.SampleDesc.Quality = 0;
	sbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	sbResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	//リソースの生成
	auto result = pDevice->CreateCommittedResource(
		&sbHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&sbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_pResource.GetAddressOf())
	);

	//ビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srViewDesc = {};
	srViewDesc.Format = DXGI_FORMAT_UNKNOWN;
	srViewDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srViewDesc.Buffer.FirstElement = firstElement;
	srViewDesc.Buffer.NumElements = numElements;
	srViewDesc.Buffer.StructureByteStride = sizeof(T);
	srViewDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	//シェーダーリソースビュー作成
	pDevice->CreateShaderResourceView(m_pResource.Get(), &srViewDesc, cpuHandle);
}

template<typename T>
inline void DX12StructuredBuffer<T>::map(UINT subResource, D3D12_RANGE* pReadRange, T** pMap)
{
	m_pResource.Get()->Map(subResource, pReadRange, (void**)pMap);
}

template<typename T>
inline void DX12StructuredBuffer<T>::unMap(UINT subResource, D3D12_RANGE* pWrittenRange)
{
	m_pResource.Get()->Unmap(subResource, pWrittenRange);
}
