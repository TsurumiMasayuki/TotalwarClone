#pragma once
#include "Graphics\DX12\Material\AbstractMaterial.h"

#include "Blockbench/BlockbenchModel.h"
#include "Graphics\DX12\DX12StructuredBuffer.h"

class DX12Texture2D;

struct DataPerCube
{
	DirectX::XMFLOAT2 m_UVArray[36];
	DirectX::XMFLOAT4X4 m_Matrix;
	unsigned int m_ObjectDataIndex;
};

struct DataPerObject
{
	DirectX::XMFLOAT4X4 m_Matrix;
};

//Blockbenchモデル用マテリアル
class BBModelMaterial
	: public AbstractMaterial
{
public:
	BBModelMaterial();

	virtual D3D12_RASTERIZER_DESC createRasterizerState() override;
	virtual void setMaterial(ID3D12GraphicsCommandList* pCommandList) override;
	virtual void initShaders() override;
	virtual void initPipelineState(ID3D12Device* pDevice) override;
	virtual void initDescriptorHeap(ID3D12Device* pDevice) override;

	void setBBModel(const BlockbenchModel& bbModel);
	void setModelCount(int modelCount);
	void initDataPerCube();

	void initDataPerObject();
	void updateDataPerObject(const std::vector<DirectX::XMMATRIX>& dataPerObject);

private:
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> g_pRootSignature;
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> g_pPipelineState;

	DX12Texture2D* m_pMainTexture;
	D3D12_GPU_DESCRIPTOR_HANDLE m_MainTextureSRV;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pDescriptorHeap;

	DX12StructuredBuffer<DataPerCube> m_DataPerCubeBuffer;
	DX12StructuredBuffer<DataPerObject> m_DataPerObjectBuffer;

	const BlockbenchModel* m_pBBModel;
	int m_ModelCount;
};