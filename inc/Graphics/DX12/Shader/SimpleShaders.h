#pragma once
#include <Graphics\DX12\DX12DescriptorRange.h>
#include <Graphics\DX12\Shader\AbstractVertexShader.h>
#include <Graphics\DX12\Shader\AbstractPixelShader.h>

//シンプル系Renderer用シェーダー
class SimpleVertexShader
	: public AbstractVertexShader
{
public:
	virtual void getInputLayout(std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElementArray, int & inputElementCount) const override;
	virtual void getRootParams(std::vector<D3D12_ROOT_PARAMETER>& rootParamsArray, int & rootParamCount) const override;

protected:
	virtual std::string getShaderFileName() const override { return "BasicVertexShader.cso"; }
};

//CircleRenderer用デフォルトシェーダ
class CirclePixelShader
	: public AbstractPixelShader
{
public:
	CirclePixelShader() {}

	virtual void getRootParams(std::vector<D3D12_ROOT_PARAMETER>& rootParamsArray, int & rootParamCount) const override;

protected:
	virtual std::string getShaderFileName() const override { return "CirclePixelShader.cso"; }
};