#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <Utility\INonCopyable.h>

template<typename T>
class DX12ConstantBuffer
	: private INonCopyable
{
public:
	void init(ID3D12Device * pDevice, const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle);

	void map(UINT subResource, D3D12_RANGE* pReadRange, T** pMap);
	void unMap(UINT subResource, D3D12_RANGE* pWrittenRange);

	D3D12_GPU_VIRTUAL_ADDRESS getGPUVirtualAddress() const { return m_pResource->GetGPUVirtualAddress(); }

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> m_pResource;
	D3D12_GPU_VIRTUAL_ADDRESS m_GPUVirtualAddress;
};

template<typename T>
inline void DX12ConstantBuffer<T>::init(ID3D12Device * pDevice, const D3D12_CPU_DESCRIPTOR_HANDLE & cpuHandle)
{
	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProperties{};
	cbHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

	//リソース設定
	D3D12_RESOURCE_DESC cbResDesc{};
	cbResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResDesc.Width = (sizeof(T) + 0xff) & ~0xff;	//256バイトアラインメント
	cbResDesc.Height = 1;
	cbResDesc.DepthOrArraySize = 1;
	cbResDesc.MipLevels = 1;
	cbResDesc.SampleDesc.Count = 1;
	cbResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT result = pDevice->CreateCommittedResource(
		&cbHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&cbResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_pResource));

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = m_pResource->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)m_pResource->GetDesc().Width;
	pDevice->CreateConstantBufferView(&cbvDesc, cpuHandle);

	m_GPUVirtualAddress = m_pResource->GetGPUVirtualAddress();
}

template<typename T>
inline void DX12ConstantBuffer<T>::map(UINT subResource, D3D12_RANGE * pReadRange, T ** pMap)
{
	m_pResource.Get()->Map(subResource, pReadRange, (void**)pMap);
}

template<typename T>
 void DX12ConstantBuffer<T>::unMap(UINT subResource, D3D12_RANGE * pWrittenRange)
{
	m_pResource.Get()->Unmap(subResource, pWrittenRange);
}
