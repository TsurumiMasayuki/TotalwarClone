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

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	ZeroMemory(&heapProp, sizeof(D3D12_HEAP_PROPERTIES));
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resourceDesc{};
	ZeroMemory(&resourceDesc, sizeof(D3D12_RESOURCE_DESC));
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = sizeof(T) * m_VertexCount;	//���_��񂪓��镪�̃T�C�Y
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//GPU���\�[�X�̐���
	HRESULT result = pDevice->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_pResource));

	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	T* pVertexMap = nullptr;
	result = m_pResource->Map(0, nullptr, (void**)&pVertexMap);
	//�S���_�ɑ΂��ēK�p
	for (UINT i = 0; i < m_VertexCount; ++i)
	{
		pVertexMap[i] = vertices[i];
	}
	//�}�b�v������
	m_pResource->Unmap(0, nullptr);

	//���_�o�b�t�@���쐬
	m_VertexBufferView.BufferLocation = m_pResource->GetGPUVirtualAddress();
	m_VertexBufferView.SizeInBytes = sizeof(T) * m_VertexCount;
	m_VertexBufferView.StrideInBytes = sizeof(T);
}

template<typename T>
void DX12VertexBuffer<T>::setData(const std::vector<T>& vertices)
{
	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	T* pVertexMap = nullptr;
	HRESULT result = m_pResource->Map(0, nullptr, (void**)&pVertexMap);
	//�S���_�ɑ΂��ēK�p
	for (int i = 0; i < m_VertexCount; ++i)
	{
		pVertexMap[i] = vertices[i];
	}
	//�}�b�v������
	m_pResource->Unmap(0, nullptr);
}
