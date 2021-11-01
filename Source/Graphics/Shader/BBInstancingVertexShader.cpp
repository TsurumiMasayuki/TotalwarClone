#include "BBInstancingVertexShader.h"

void BBInstancingVertexShader::getInputLayout(std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElementArray, int& inputElementCount) const
{
	//頂点バッファ
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

	//インスタンスバッファ

	//行列を4つに分けて送る
	D3D12_INPUT_ELEMENT_DESC element6 =
	{ "INSTANCEMAT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };
	D3D12_INPUT_ELEMENT_DESC element7 =
	{ "INSTANCEMAT", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };
	D3D12_INPUT_ELEMENT_DESC element8 =
	{ "INSTANCEMAT", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };
	D3D12_INPUT_ELEMENT_DESC element9 =
	{ "INSTANCEMAT", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };

	//色
	D3D12_INPUT_ELEMENT_DESC element10 =
	{ "INSTANCECOLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };

	inputElementArray.emplace_back(element1);
	inputElementArray.emplace_back(element2);
	inputElementArray.emplace_back(element3);
	inputElementArray.emplace_back(element4);
	inputElementArray.emplace_back(element5);

	inputElementArray.emplace_back(element6);
	inputElementArray.emplace_back(element7);
	inputElementArray.emplace_back(element8);
	inputElementArray.emplace_back(element9);

	inputElementArray.emplace_back(element10);

	//面ごとのUVの原点
	for (unsigned int i = 0; i < 3; i++)
	{
		D3D12_INPUT_ELEMENT_DESC element =
		{ "INSTANCEUVORIGINS", i, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };
		inputElementArray.emplace_back(element);
	}

	//面ごとのUVのサイズ
	for (unsigned int i = 0; i < 3; i++)
	{
		D3D12_INPUT_ELEMENT_DESC element =
		{ "INSTANCEUVSIZES", i, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };
		inputElementArray.emplace_back(element);
	}

	//手動+ループ数分
	inputElementCount = 10 + 6;
}

void BBInstancingVertexShader::getRootParams(std::vector<D3D12_ROOT_PARAMETER>& rootParamsArray, int& rootParamCount) const
{
	D3D12_ROOT_PARAMETER rootParams[1] = {};
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;		//種類
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;		//全てのシェーダーから見える
	rootParams[0].Descriptor.ShaderRegister = 0;						//スロット指定
	rootParams[0].Descriptor.RegisterSpace = 0;

	rootParamsArray.emplace_back(rootParams[0]);
	rootParamCount = _countof(rootParams);
}

std::string BBInstancingVertexShader::getShaderFileName() const
{
	return "BBInstancingVertexShader.cso";
}
