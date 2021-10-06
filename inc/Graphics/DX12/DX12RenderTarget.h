#pragma once
#include <wrl.h>
#include <vector>
#include <d3d12.h>
#include <Utility\INonCopyable.h>
#include <Graphics\DX12\DX12Texture2D.h>

class DX12RenderTarget
	: private INonCopyable
{
public:
	DX12RenderTarget();

	void init(ID3D12Device* pDevice, int width, int height, const float clearColor[4], int renderTargetCount = 1);
	void initWithResource(ID3D12Device* pDevice, ID3D12Resource* pResource, int width, int height, const float clearColor[4]);
	D3D12_CPU_DESCRIPTOR_HANDLE getRTVCPUDescriptorHandle();
	D3D12_CPU_DESCRIPTOR_HANDLE getDSVCPUDescriptorHandle();
	void getClearColor(float clearColor[4]) const;
	const Microsoft::WRL::ComPtr<ID3D12Resource>& getResource(int index);
	DX12Texture2D& getTexture(int index) { return m_Textures.at(index); }
	int getRenderTargetCount() const;

	const D3D12_VIEWPORT& getViewPort() const;
	const D3D12_RECT& getScissorRect() const;

private:
	int m_Width;
	int m_Height;
	int m_RenderTargetCount;
	float m_ClearColor[4];
	bool m_IsInitWithResource;

	std::vector<DX12Texture2D> m_Textures;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_pTextureResource;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pSRVHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pRTVHeap;

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_DSVResources;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pDSVHeap;

	D3D12_VIEWPORT m_ViewPort;
	D3D12_RECT m_ScissorRect;
};