#pragma once
#include <Graphics\DX12\DX12DescriptorRange.h>
#include <Graphics\DX12\Shader\AbstractVertexShader.h>
#include <Graphics\DX12\Shader\AbstractPixelShader.h>
#include <Graphics\DX12\Shader\AbstractGeometryShader.h>

//�X�v���C�g�p�f�t�H���g�V�F�[�_
class SpriteVertexShader
	: public AbstractVertexShader
{
public:
	virtual void getInputLayout(std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElementArray, int & inputElementCount) const override;
	virtual void getRootParams(std::vector<D3D12_ROOT_PARAMETER>& rootParamsArray, int & rootParamCount) const override;

protected:
	virtual std::string getShaderFileName() const override { return "BasicVertexShader.cso"; }
};

//�X�v���C�g�p�f�t�H���g�V�F�[�_
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

//���b�V���p�f�t�H���g�V�F�[�_
class MeshVertexShader
	: public AbstractVertexShader
{
public:
	virtual void getInputLayout(std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElementArray, int & inputElementCount) const override;
	virtual void getRootParams(std::vector<D3D12_ROOT_PARAMETER>& rootParamsArray, int & rootParamCount) const override;

protected:
	virtual std::string getShaderFileName() const override { return "MeshVertexShader.cso"; }
};

//���b�V���p�f�t�H���g�V�F�[�_
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

//�p�[�e�B�N���p���_�V�F�[�_
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

//�p�[�e�B�N���p�W�I���g���V�F�[�_
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

//�p�[�e�B�N���s�N�Z���V�F�[�_
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

//�C���X�^���V���O�p���_�V�F�[�_�[
class InstancingVertexShader
	: public AbstractVertexShader
{
public:
	virtual void getInputLayout(std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElementArray, int& inputElementCount) const override;
	virtual void getRootParams(std::vector<D3D12_ROOT_PARAMETER>& rootParamsArray, int& rootParamCount) const override;

protected:
	virtual std::string getShaderFileName() const override;
};

//�C���X�^���V���O�p�s�N�Z���V�F�[�_�[
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