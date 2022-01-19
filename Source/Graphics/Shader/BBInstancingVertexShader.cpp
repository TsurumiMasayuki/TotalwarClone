#include "BBInstancingVertexShader.h"

void BBInstancingVertexShader::getInputLayout(std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElementArray, int& inputElementCount) const
{
	//���_�o�b�t�@
	D3D12_INPUT_ELEMENT_DESC element1 =
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	D3D12_INPUT_ELEMENT_DESC element2 =
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	D3D12_INPUT_ELEMENT_DESC element3 =
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	D3D12_INPUT_ELEMENT_DESC element4 =
	{ "BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	D3D12_INPUT_ELEMENT_DESC element5 =
	{ "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	inputElementArray.emplace_back(element1);
	inputElementArray.emplace_back(element2);
	inputElementArray.emplace_back(element3);
	inputElementArray.emplace_back(element4);
	inputElementArray.emplace_back(element5);

	inputElementCount = 5;
}

void BBInstancingVertexShader::getRootParams(std::vector<D3D12_ROOT_PARAMETER>& rootParamsArray, int& rootParamCount) const
{
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;		//���
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;		//�S�ẴV�F�[�_�[���猩����
	rootParams[0].Descriptor.ShaderRegister = 0;						//�X���b�g�w��
	rootParams[0].Descriptor.RegisterSpace = 0;

	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;		//���
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;		//�S�ẴV�F�[�_�[���猩����
	rootParams[1].Descriptor.ShaderRegister = 1;						//�X���b�g�w��
	rootParams[1].Descriptor.RegisterSpace = 0;

	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;		//���
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;		//�S�ẴV�F�[�_�[���猩����
	rootParams[2].Descriptor.ShaderRegister = 2;						//�X���b�g�w��
	rootParams[2].Descriptor.RegisterSpace = 0;

	rootParamsArray.emplace_back(rootParams[0]);
	rootParamsArray.emplace_back(rootParams[1]);
	rootParamsArray.emplace_back(rootParams[2]);
	rootParamCount = _countof(rootParams);
}

std::string BBInstancingVertexShader::getShaderFileName() const
{
	return "BBInstancingVertexShader.cso";
}
