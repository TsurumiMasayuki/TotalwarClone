#pragma once
#include <Graphics\DX12\DX12DescriptorRange.h>
#include <Graphics\DX12\Shader\AbstractVertexShader.h>
#include <Graphics\DX12\Shader\AbstractPixelShader.h>

class GaussianBlurPixelShader
	: public AbstractPixelShader
{
public:
	GaussianBlurPixelShader() : m_DescRangeSRV(0) {}

	virtual void getRootParams(std::vector<D3D12_ROOT_PARAMETER>& rootParamsArray, int& rootParamCount) const override;
	virtual std::string getShaderFileName() const override;

private:
	DX12DescriptorRangeSRV m_DescRangeSRV;
};

class LuminanceExtractPixelShader
	: public AbstractPixelShader
{
public:
	LuminanceExtractPixelShader() : m_DescRangeSRV(0) {}

	virtual void getRootParams(std::vector<D3D12_ROOT_PARAMETER>& rootParamsArray, int& rootParamCount) const override;
	virtual std::string getShaderFileName() const override;

private:
	DX12DescriptorRangeSRV m_DescRangeSRV;
};

//加算合成シェーダー
class AdditivePixelShader
	: public AbstractPixelShader
{
public:
	AdditivePixelShader() : m_DescRangeSRV1(0, 0), m_DescRangeSRV2(1, 0) {}

	virtual void getRootParams(std::vector<D3D12_ROOT_PARAMETER>& rootParamsArray, int& rootParamCount) const override;
	virtual std::string getShaderFileName() const override;

private:
	DX12DescriptorRangeSRV m_DescRangeSRV1;
	DX12DescriptorRangeSRV m_DescRangeSRV2;
};