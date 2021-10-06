#pragma once
#include <d3d12.h>

struct DX12DescriptorRangeCBV
	: public D3D12_DESCRIPTOR_RANGE
{
	DX12DescriptorRangeCBV(int shaderRegister)
	{
		RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		NumDescriptors = 1;
		BaseShaderRegister = shaderRegister;
		RegisterSpace = 0;
		OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	}
};

struct DX12DescriptorRangeSRV
	: public D3D12_DESCRIPTOR_RANGE
{
	DX12DescriptorRangeSRV(int shaderRegister, int registerSpace = 0)
	{
		RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		NumDescriptors = 1;
		BaseShaderRegister = shaderRegister;
		RegisterSpace = registerSpace;
		OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	}
};