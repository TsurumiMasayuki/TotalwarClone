#pragma once
#include <d3d12.h>
#include <string>
#include <vector>
#include <Utility\INonCopyable.h>

class AbstractGeometryShader
	: public INonCopyable
{
public:
	AbstractGeometryShader();
	~AbstractGeometryShader();

	void init();
	void getShaderByteCode(unsigned char** ppShaderByteCode, size_t& byteCodeLength) const;

	virtual void getRootParams(std::vector<D3D12_ROOT_PARAMETER>& rootParamsArray, int& rootParamCount) const = 0;

protected:
	virtual std::string getShaderFileName() const = 0;

protected:
	unsigned char* m_pShaderByteCode;
	size_t m_ByteCodeLength;
};