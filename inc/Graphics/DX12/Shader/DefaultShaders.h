#pragma once
#include <Graphics\DX12\DX12DescriptorRange.h>
#include <Graphics\DX12\Shader\AbstractVertexShader.h>
#include <Graphics\DX12\Shader\AbstractPixelShader.h>
#include <Graphics\DX12\Shader\AbstractGeometryShader.h>

//スプライト用デフォルトシェーダ
class SpriteVertexShader
	: public AbstractVertexShader
{
public:
	virtual void getInputLayout(std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElementArray, int & inputElementCount) const override;
	virtual void getRootParams(std::vector<D3D12_ROOT_PARAMETER>& rootParamsArray, int & rootParamCount) const override;

protected:
	virtual std::string getShaderFileName() const override { return "BasicVertexShader.cso"; }
};

//スプライト用デフォルトシェーダ
class SpritePixelShader
	: public AbstractPixelShader
{
public:
	SpritePixelShader() : m_DescRangeSRV(0)	{}

	virtual void getRootParams(std::vector<D3D12_ROOT_PARAMETER>& rootParamsArray, int & rootParamCount) const override;

protected:
	virtual std::string getShaderFileName() const override { return "BasicPixelShader.cso"; }

private:
	DX12DescriptorRangeSRV m_DescRangeSRV;
};

//メッシュ用デフォルトシェーダ
class MeshVertexShader
	: public AbstractVertexShader
{
public:
	virtual void getInputLayout(std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElementArray, int & inputElementCount) const override;
	virtual void getRootParams(std::vector<D3D12_ROOT_PARAMETER>& rootParamsArray, int & rootParamCount) const override;

protected:
	virtual std::string getShaderFileName() const override { return "MeshVertexShader.cso"; }
};

//メッシュ用デフォルトシェーダ
class MeshPixelShader
	: public AbstractPixelShader
{
public:
	MeshPixelShader() : m_DescRangeSRV(0) {}

	virtual void getRootParams(std::vector<D3D12_ROOT_PARAMETER>& rootParamsArray, int & rootParamCount) const override;

protected:
	virtual std::string getShaderFileName() const override { return "MeshPixelShader.cso"; }

private:
	DX12DescriptorRangeSRV m_DescRangeSRV;
};

//パーティクル用頂点シェーダ
class ParticleVertexShader
	: public AbstractVertexShader
{
public:
	ParticleVertexShader() {}

	virtual void getInputLayout(std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElementArray, int& inputElementCount) const override;
	virtual void getRootParams(std::vector<D3D12_ROOT_PARAMETER>& rootParamsArray, int& rootParamCount) const override;

protected:
	virtual std::string getShaderFileName() const override { return "ParticleVertexShader.cso"; }
};

//パーティクル用ジオメトリシェーダ
class ParticleGeometryShader
	: public AbstractGeometryShader
{
public:
	ParticleGeometryShader() : m_DescRangeSRV(0) {}

	virtual void getRootParams(std::vector<D3D12_ROOT_PARAMETER>& rootParamsArray, int& rootParamCount) const override;

protected:
	virtual std::string getShaderFileName() const override { return "ParticleGeometryShader.cso"; }

private:
	DX12DescriptorRangeSRV m_DescRangeSRV;
};

//パーティクルピクセルシェーダ
class ParticlePixelShader
	: public AbstractPixelShader
{
public:
	ParticlePixelShader() : m_DescRangeSRV(0) {}

	virtual void getRootParams(std::vector<D3D12_ROOT_PARAMETER>& rootParamsArray, int& rootParamCount) const override;

protected:
	virtual std::string getShaderFileName() const override { return "ParticlePixelShader.cso"; }

private:
	DX12DescriptorRangeSRV m_DescRangeSRV;
};

//インスタンシング用頂点シェーダー
class InstancingVertexShader
	: public AbstractVertexShader
{
public:
	virtual void getInputLayout(std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElementArray, int& inputElementCount) const override;
	virtual void getRootParams(std::vector<D3D12_ROOT_PARAMETER>& rootParamsArray, int& rootParamCount) const override;

protected:
	virtual std::string getShaderFileName() const override;
};

//インスタンシング用ピクセルシェーダー
class InstancingPixelShader
	: public AbstractPixelShader
{
public:
	InstancingPixelShader() : m_DescRangeSRV(0) {}

	virtual void getRootParams(std::vector<D3D12_ROOT_PARAMETER>& rootParamsArray, int& rootParamCount) const override;

protected:
	virtual std::string getShaderFileName() const override { return "InstancingPixelShader.cso"; }

private:
	DX12DescriptorRangeSRV m_DescRangeSRV;
};