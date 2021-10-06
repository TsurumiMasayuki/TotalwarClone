#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>

class DX12GraphicsCore
{
public:
	static Microsoft::WRL::ComPtr<ID3D12Device> g_pDevice;
	static Microsoft::WRL::ComPtr<IDXGIFactory6> g_pDXGIFactory;
	static Microsoft::WRL::ComPtr<IDXGISwapChain4>	g_pDXGISwapChain;
	static Microsoft::WRL::ComPtr<ID3D12CommandQueue> g_pCommandQueue;

	static void initDX12(const HWND& hwnd);
	static void shutdownDX12();
};