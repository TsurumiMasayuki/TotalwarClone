#pragma once
#include <wrl.h>
#include <d3d11on12.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#include <d2d1_3.h>
#include <dwrite.h>

class DX12GraphicsCore
{
public:
	static Microsoft::WRL::ComPtr<ID3D11Device> g_pD3D11Device;
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext> g_pD3D11DeviceContext;
	static Microsoft::WRL::ComPtr<ID3D11On12Device> g_pD3D11On12Device;

	static Microsoft::WRL::ComPtr<ID3D12Device> g_pDevice;
	static Microsoft::WRL::ComPtr<IDXGIFactory6> g_pDXGIFactory;
	static Microsoft::WRL::ComPtr<IDXGISwapChain3>	g_pDXGISwapChain;
	static Microsoft::WRL::ComPtr<ID3D12CommandQueue> g_pCommandQueue;

	static Microsoft::WRL::ComPtr<ID2D1Factory3> g_pD2DFactory;
	static Microsoft::WRL::ComPtr<ID2D1Device2> g_pD2DDevice;
	static Microsoft::WRL::ComPtr<ID2D1DeviceContext2> g_pD2DDeviceContext;
	static Microsoft::WRL::ComPtr<IDWriteFactory> g_pDWriteFactory;

	static void initDX12(const HWND& hwnd);
	static void shutdownDX12();
};