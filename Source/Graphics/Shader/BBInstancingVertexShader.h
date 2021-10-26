#pragma once
#include "Graphics\DX12\Shader\AbstractVertexShader.h"

class BBInstancingVertexShader
	: public AbstractVertexShader
{
public:
	virtual void getInputLayout(std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElementArray, int& inputElementCount) const override;
	virtual void getRootParams(std::vector<D3D12_ROOT_PARAMETER>& rootParamsArray, int& rootParamCount) const override;
	virtual std::string getShaderFileName() const override;
};