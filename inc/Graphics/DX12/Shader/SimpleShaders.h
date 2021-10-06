#pragma once
#include <Graphics\DX12\DX12DescriptorRange.h>
#include <Graphics\DX12\Shader\AbstractVertexShader.h>
#include <Graphics\DX12\Shader\AbstractPixelShader.h>

//�V���v���nRenderer�p�V�F�[�_�[
class SimpleVertexShader
	: public AbstractVertexShader
{
public:
	virtual void getInputLayout(std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElementArray, int & inputElementCount) const override;
	virtual void getRootParams(std::vector<D3D12_ROOT_PARAMETER>& rootParamsArray, int & rootParamCount) const override;

protected:
	virtual std::string getShaderFileName() const override { return "BasicVertexShader.cso"; }
};

//CircleRenderer�p�f�t�H���g�V�F�[�_
class CirclePixelShader
	: public AbstractPixelShader
{
public:
	CirclePixelShader() {}

	virtual void getRootParams(std::vector<D3D12_ROOT_PARAMETER>& rootParamsArray, int & rootParamCount) const override;

protected:
	virtual std::string getShaderFileName() const override { return "CirclePixelShader.cso"; }
};