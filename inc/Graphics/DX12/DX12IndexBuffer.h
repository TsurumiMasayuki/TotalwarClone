#pragma once
#include <wrl.h>
#include <vector>
#include <d3d12.h>
#include <Utility\INonCopyable.h>

class DX12IndexBuffer
	: private INonCopyable
{
public:
	void init(ID3D12Device * pDevice, const std::vector<USHORT>& indices);

	UINT getIndexCount() const { return m_IndexCount; }
	const D3D12_INDEX_BUFFER_VIEW& getIndexBufferView() const { return m_IndexBuffer; }

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> m_pResource;
	D3D12_INDEX_BUFFER_VIEW m_IndexBuffer;
	UINT m_IndexCount;
};