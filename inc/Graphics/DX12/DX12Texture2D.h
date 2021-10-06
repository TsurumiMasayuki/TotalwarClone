#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <string>
#include <DirectXMath.h>

class DX12Texture2D
{
public:
	void initWithData(const DirectX::XMFLOAT4* initData, int dataWidth, int dataHeight);
	void initWithWICFile(const std::wstring& filePath);

	const Microsoft::WRL::ComPtr<ID3D12Resource>& getResource() const { return m_pResource; }
	const DXGI_FORMAT& getFormat() const;

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> m_pResource;
	DXGI_FORMAT m_Format;
};